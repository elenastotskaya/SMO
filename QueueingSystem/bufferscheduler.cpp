#include "bufferscheduler.h"

BufferScheduler::BufferScheduler(SourceArray& inputArray, Buffer& outputBuf)
    :sources(inputArray),
    buf(outputBuf)
{
    generatedCount = new unsigned int[sources.getSize()]{};
    refusedCount = new unsigned int[sources.getSize()]{};
    generatedTime = new double [sources.getSize()];
    for (unsigned int i = 0; i < sources.getSize(); ++i) {
        generatedTime[i] = 0.0;
    }
}

BufferScheduler::~BufferScheduler()
{
    delete[] generatedCount;
    delete[] refusedCount;
    delete[] generatedTime;
}

void BufferScheduler::processRequest(unsigned int selectionMode)
{
    double minTime;
    switch (selectionMode) {
    case 1:
        minTime = sources.chooseMinAndGenerate();
        break;
    case 2:
        minTime = sources.chooseMin();
        break;
    case 3:
        minTime = sources.chooseRemainingMin();
        break;
    }
    unsigned int minInd = sources.getMinIndex();
    Request newReq(minTime, minInd);
    if (!buf.add(newReq)) {
        ++(refusedCount[sources.getPrevMinIndex()]);
    }
}

void BufferScheduler::reset()
{
    sources.reset();
    buf.reset();
    for (unsigned int i = 0; i < sources.getSize(); ++i) {
        generatedTime[i] = 0.0;
        generatedCount[i] = 0;
        refusedCount[i] = 0;
    }
}

unsigned int BufferScheduler::getGenerated(unsigned int index)
{
    return sources.getGenerated(index);
}

unsigned int BufferScheduler::getRefused(unsigned int index)
{
    return refusedCount[index];
}

double BufferScheduler::getGeneratedTime(unsigned int index)
{
    return sources.getGeneratedTime(index);
}

unsigned int BufferScheduler::getCurrentSourceIndex()
{
    return sources.getMinIndex();
}

unsigned int BufferScheduler::getCurrentGenerated()
{
    return sources.getCurrentGenerated();
}

unsigned int BufferScheduler::getCurrentRefused()
{
    return refusedCount[sources.getPrevMinIndex()];
}

double BufferScheduler::getCurrentGeneratedTime()
{
    return sources.getCurrentGeneratedTime();
}

void BufferScheduler::initBufferOutput()
{
    buf.initOutput();
}

unsigned int BufferScheduler::getCurrentBufferIndex()
{
    return buf.getCurrentIndex();
}

double BufferScheduler::getCurrentBufferTime()
{
    return buf.getCurrentTime();
}

void BufferScheduler::stepBufferOutput()
{
    buf.stepOutput();
}

double BufferScheduler::getMinInitial()
{
    return sources.getMinInitial();
}
