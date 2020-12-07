#include "selectionscheduler.h"

SelectionScheduler::SelectionScheduler(Buffer& inputBuf, DeviceList& outputList, unsigned int sourceCount)
    :buf(inputBuf),
    devices(outputList),
    isSkipping(false),
    currentSourceNum(0),
    currentGeneratedTime(0.0),
    statsArraySize(sourceCount)
{
    finalProcessTime = new double[sourceCount];
    finalProcessTimeSquared = new double[sourceCount];
    finalWaitingTime = new double[sourceCount];
    finalWaitingTimeSquared = new double[sourceCount];
    for (unsigned int i = 0; i < sourceCount; ++i) {
        finalProcessTime[i] = 0.0;
        finalProcessTimeSquared[i] = 0.0;
        finalWaitingTime[i] = 0.0;
        finalWaitingTimeSquared[i] = 0.0;
    }
}

SelectionScheduler::~SelectionScheduler()
{
    delete[] finalProcessTime;
    delete[] finalProcessTimeSquared;
    delete[] finalWaitingTime;
    delete[] finalWaitingTimeSquared;
}

void SelectionScheduler::processRequest()
{
    lastReqTime = buf.getTailTime();
    if (lastReqTime > 0) {
       if (devices.getSize() > 1) {            
            do {
                if (devices.getCurrentTEnd() < lastReqTime) {
                    currentSourceNum = buf.getHeadSource();
                    currentGeneratedTime = buf.getHeadTime();
                    devices.addRequest(buf.release());
                    isSkipping = false;
                    return;
                }
                devices.stepCurrent();
            } while(!(devices.isLooped()));
            isSkipping = true;
        } else {
            if (devices.getCurrentTEnd() < lastReqTime) {
                currentSourceNum = buf.getHeadSource();
                currentGeneratedTime = buf.getHeadTime();
                devices.addRequest(buf.release());
                isSkipping = false;
            } else {
                isSkipping = true;
            }
        }
    }
}

void SelectionScheduler::processLastFromBuffer()
{
    lastReqTime = buf.getTailTime();
    while (lastReqTime > 0) {
        currentSourceNum = buf.getHeadSource();
        currentGeneratedTime = buf.getHeadTime();
        devices.addRequestByMinTime(buf.release());
        lastReqTime = buf.getTailTime();
        finalProcessTime[currentSourceNum] += devices.getLastTEnd() - devices.getLastTStart();
        finalProcessTimeSquared[currentSourceNum] += (devices.getLastTEnd() - devices.getLastTStart())*(devices.getLastTEnd() - devices.getLastTStart());
        finalWaitingTime[currentSourceNum] += devices.getLastTStart() - currentGeneratedTime;
        finalWaitingTimeSquared[currentSourceNum] += (devices.getLastTStart() - currentGeneratedTime)*(devices.getLastTStart() - currentGeneratedTime);
    }
}

bool SelectionScheduler::processLastStepByStep()
{
    lastReqTime = buf.getTailTime();
    if (lastReqTime > 0) {
        currentSourceNum = buf.getHeadSource();
        currentGeneratedTime = buf.getHeadTime();
        devices.addRequestByMinTime(buf.release());
        //lastReqTime = buf.getTailTime();
        finalProcessTime[currentSourceNum] += devices.getLastTEnd() - devices.getLastTStart();
        finalProcessTimeSquared[currentSourceNum] += (devices.getLastTEnd() - devices.getLastTStart())*(devices.getLastTEnd() - devices.getLastTStart());
        finalWaitingTime[currentSourceNum] += devices.getLastTStart() - currentGeneratedTime;
        finalWaitingTimeSquared[currentSourceNum] += (devices.getLastTStart() - currentGeneratedTime)*(devices.getLastTStart() - currentGeneratedTime);
        return false;
    }
    return true;
}

void SelectionScheduler::reset()
{
    devices.reset();
    isSkipping = false;
    for (unsigned int i = 0; i < statsArraySize; ++i) {
        finalProcessTime[i] = 0.0;
        finalProcessTimeSquared[i] = 0.0;
        finalWaitingTime[i] = 0.0;
        finalWaitingTimeSquared[i] = 0.0;
    }
}

bool SelectionScheduler::getSkipping()
{
    return isSkipping;
}

double SelectionScheduler::getCurrentTStart()
{
    return devices.getCurrentTStart();
}

double SelectionScheduler::getCurrentTEnd()
{
    return devices.getCurrentTEnd();
}

double SelectionScheduler::getLastTStart()
{
    return devices.getLastTStart();
}

double SelectionScheduler::getLastTEnd()
{
    return devices.getLastTEnd();
}

unsigned int SelectionScheduler::getLastIndex()
{
    return devices.getLastIndex();
}

unsigned int SelectionScheduler::getCurrentSource()
{
    return currentSourceNum;
}

double SelectionScheduler::getCurrentGeneratedTime()
{
    return currentGeneratedTime;
}

double SelectionScheduler::collectDeviceTotalTime()
{
    double deviceTime = devices.getCurrentTotalTime();
    devices.stepCurrent();
    return deviceTime;
}

double SelectionScheduler::getFinalProcessTime(unsigned int index)
{
    return finalProcessTime[index];
}

double SelectionScheduler::getFinalProcessTimeSquared(unsigned int index)
{
    return finalProcessTimeSquared[index];
}

double SelectionScheduler::getFinalWaitingTime(unsigned int index)
{
    return finalWaitingTime[index];
}

double SelectionScheduler::getFinalWaitingTimeSquared(unsigned int index)
{
    return finalWaitingTimeSquared[index];
}
