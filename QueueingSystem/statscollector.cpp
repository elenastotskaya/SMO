#include "statscollector.h"
#include <cmath>
#include "stepbystepwindow.h"

StatsCollector::StatsCollector(BufferScheduler& bufSched, SelectionScheduler& selSched, unsigned int inputSourceCount, unsigned int inputDeviceCount, unsigned int inputBufferSize, bool mode)
    :bufScheduler(bufSched),
    selScheduler(selSched),
    sourceCount(inputSourceCount),
    deviceCount(inputDeviceCount),
    bufferSize(inputBufferSize),
    simulationMode(mode),
    maxRequestCount(MAX_REQ_COUNT_INIT),
    pPrev(DUMMY_PROB),
    precise(false),
    tempIndexCount(0),
    generatedCount(inputSourceCount),
    probRefusal(inputSourceCount),
    avgSystem(inputSourceCount),
    avgWaiting(inputSourceCount),
    avgProcessing(inputSourceCount),
    dispWaiting(inputSourceCount),
    dispProcessing(inputSourceCount),
    operatingRatio(inputDeviceCount)
{
}

void StatsCollector::collectFromDevices()
{
    double currentGeneratedTime = selScheduler.getCurrentGeneratedTime();
    double currentTStart = selScheduler.getLastTStart();
    double currentTEnd = selScheduler.getLastTEnd();
    unsigned int currentSourceIndex = selScheduler.getCurrentSource();
    avgSystem[currentSourceIndex] += (currentTEnd - currentGeneratedTime);
    avgWaiting[currentSourceIndex] += (currentTStart - currentGeneratedTime);
    avgProcessing[currentSourceIndex] += (currentTEnd - currentTStart);
    dispWaiting[currentSourceIndex] += (currentTStart - currentGeneratedTime)*(currentTStart - currentGeneratedTime);
    dispProcessing[currentSourceIndex] += (currentTEnd - currentTStart)*(currentTEnd - currentTStart);
}

void StatsCollector::collect()
{
    double maxDeviceTime = 0.0;
    for(unsigned int i = 0; i < deviceCount; ++i) {
        if (maxDeviceTime < selScheduler.getCurrentTEnd()) {
            maxDeviceTime = selScheduler.getCurrentTEnd();
        }
        operatingRatio[i] = selScheduler.collectDeviceTotalTime();
    }
    double iterationTime = maxDeviceTime - bufScheduler.getMinInitial();
    for(unsigned int i = 0; i < deviceCount; ++i) {
        operatingRatio[i] /= iterationTime;
    }
    for(unsigned int i = 0; i < sourceCount; ++i) {
        if (generatedCount[i] > 0) {
            probRefusal[i] = static_cast<double>(bufScheduler.getRefused(i))/generatedCount[i];
            avgSystem[i] += selScheduler.getFinalProcessTime(i) + selScheduler.getFinalWaitingTime(i);
            avgSystem[i] /= generatedCount[i];
            avgWaiting[i] += selScheduler.getFinalWaitingTime(i);
            avgWaiting[i] /= generatedCount[i];
            avgProcessing[i] += selScheduler.getFinalProcessTime(i);
            avgProcessing[i] /= generatedCount[i];
            dispWaiting[i] += selScheduler.getFinalWaitingTimeSquared(i);
            dispWaiting[i] /= generatedCount[i];
            dispWaiting[i] -= avgWaiting[i]*avgWaiting[i];
            dispProcessing[i] += selScheduler.getFinalProcessTimeSquared(i);
            dispProcessing[i] /= generatedCount[i];
            dispProcessing[i] -= avgProcessing[i]*avgProcessing[i];
            dispProcessing[i] = dispProcessing[i];
        }
    }
}

void StatsCollector::innerLoop()
{
    do {
        generatedSum = 0;
        bufScheduler.processRequest(1);
        selScheduler.processRequest();
        for(unsigned int i = 0; i < sourceCount; ++i) {
            generatedCount[i] = bufScheduler.getGenerated(i);
        }
        if (!selScheduler.getSkipping()) {
            collectFromDevices();
        }
        for (unsigned int i = 0; i < sourceCount; ++i) {
            generatedSum += generatedCount[i];
        }
    } while (generatedSum < maxRequestCount);
    bufScheduler.processRequest(2);
    selScheduler.processRequest();
    if (!selScheduler.getSkipping()) {
        collectFromDevices();
    }
    for (unsigned int i = 0; i < sourceCount - 1; ++i) {
        bufScheduler.processRequest(3);
        selScheduler.processRequest();
        if (!selScheduler.getSkipping()) {
            collectFromDevices();
        }
    }
    selScheduler.processLastFromBuffer();
    collect();
    if ((abs(probRefusal[0] - pPrev) < pPrev * delta) || (pPrev < MIN_P && probRefusal[0] < MIN_P && pPrev >= 0)) {
        precise = true;
        return;
    }
    pPrev = probRefusal[0];
    if (probRefusal[0] > MIN_P) {
        maxRequestCount = static_cast<unsigned int>(round(t*t*(1 - probRefusal[0])/(probRefusal[0]*delta*delta)));
    } else {
        maxRequestCount = MAX_N;
    }
    bufScheduler.reset();
    selScheduler.reset();
    for (unsigned int i = 0; i < sourceCount; ++i) {
        probRefusal[i] = 0.0;
        avgSystem[i] = 0.0;
        avgWaiting[i] = 0.0;
        avgProcessing[i] = 0.0;
        dispWaiting[i] = 0.0;
        dispProcessing[i] = 0.0;
    }
    tempIndexCount = 0;
}

void StatsCollector::display()
{
    finalStats = new FinalStatsWindow(sourceCount, deviceCount, generatedCount, probRefusal, avgSystem,
        avgWaiting, avgProcessing, dispWaiting, dispProcessing, operatingRatio);
    finalStats->exec();
    delete finalStats;
}

void StatsCollector::run()
{
    if (!simulationMode) {
        while (!precise) {
            innerLoop();
        }
        display();
    } else {
        StepByStepWindow window(*this, sourceCount, bufferSize, deviceCount);
        window.exec();
    }
}

void StatsCollector::initNormalStep()
{
    generatedSum = 0;
    bufScheduler.processRequest(1);
    for(unsigned int i = 0; i < sourceCount; ++i) {
        generatedCount[i] = bufScheduler.getGenerated(i);
    }
    for (unsigned int i = 0; i < sourceCount; ++i) {
        generatedSum += generatedCount[i];
    }
}

bool StatsCollector::processNormalStep()
{
    selScheduler.processRequest();
    if (!selScheduler.getSkipping()) {
        collectFromDevices();
    }
    if (generatedSum >= maxRequestCount) {
        return true;
    }
    return false;
}

void StatsCollector::initLastMinStep()
{
    bufScheduler.processRequest(2);
}

void StatsCollector::processLastMinStep()
{
    selScheduler.processRequest();
    if (!selScheduler.getSkipping()) {
        collectFromDevices();
    }
}

void StatsCollector::initHangingSourceStep()
{
    bufScheduler.processRequest(3);
}

bool StatsCollector::processHangingSourceStep()
{
    selScheduler.processRequest();
    if (!selScheduler.getSkipping()) {
        collectFromDevices();
    }
    ++tempIndexCount;
    if (tempIndexCount == sourceCount - 1) {
        tempIndexCount = 0;
        return true;
    }
    return false;
}

bool StatsCollector::processLastQueueStep()
{
    return selScheduler.processLastStepByStep();
}

bool StatsCollector::collectIterationStats()
{
    collect();
    display();
    if ((abs(probRefusal[0] - pPrev) < pPrev * delta) || (pPrev < MIN_P && probRefusal[0] < MIN_P && pPrev >= 0)) {
        precise = true;
        return true;
    }
    pPrev = probRefusal[0];
    if (probRefusal[0] > MIN_P) {
        maxRequestCount = static_cast<unsigned int>(round(t*t*(1 - probRefusal[0])/(probRefusal[0]*delta*delta)));
    } else {
        maxRequestCount = MAX_N;
    }
    bufScheduler.reset();
    selScheduler.reset();
    for (unsigned int i = 0; i < sourceCount; ++i) {
        probRefusal[i] = 0.0;
        avgSystem[i] = 0.0;
        avgWaiting[i] = 0.0;
        avgProcessing[i] = 0.0;
        dispWaiting[i] = 0.0;
        dispProcessing[i] = 0.0;
    }
    return false;
}

double StatsCollector::getSourceTime(int index)
{
    return bufScheduler.getGeneratedTime(index);
}

unsigned int StatsCollector::getGeneratedCount(int index)
{
    return bufScheduler.getGenerated(index);
}

unsigned int StatsCollector::getRefusedCount(int index)
{
    return bufScheduler.getRefused(index);
}

unsigned int StatsCollector::getCurrentSourceIndex()
{
    return bufScheduler.getCurrentSourceIndex();
}

double StatsCollector::getCurrentSourceTime()
{
    return bufScheduler.getCurrentGeneratedTime();
}

unsigned int StatsCollector::getCurrentGeneratedCount()
{
    return bufScheduler.getCurrentGenerated();
}

unsigned int StatsCollector::getCurrentRefusedCount()
{
    return bufScheduler.getCurrentRefused();
}

unsigned int StatsCollector::getCurrentDeviceIndex()
{
    return selScheduler.getLastIndex();
}

double StatsCollector::getCurrentDeviceStartTime()
{
    return selScheduler.getLastTStart();
}

double StatsCollector::getCurrentDeviceEndTime()
{
    return selScheduler.getLastTEnd();
}

void StatsCollector::initBufferOutput()
{
    bufScheduler.initBufferOutput();
}

unsigned int StatsCollector::getCurrentBufferIndex()
{
    return bufScheduler.getCurrentBufferIndex();
}

double StatsCollector::getCurrentBufferTime()
{
    return bufScheduler.getCurrentBufferTime();
}

void StatsCollector::stepBufferOutput()
{
    bufScheduler.stepBufferOutput();
}
