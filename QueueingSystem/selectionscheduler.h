#ifndef SELECTIONSCHEDULER_H
#define SELECTIONSCHEDULER_H

#include "buffer.h"
#include "devicelist.h"

class SelectionScheduler
{
public:
    SelectionScheduler(Buffer& inputBuf, DeviceList& outputList, unsigned int sourceCount);
    ~SelectionScheduler();
    void processRequest();
    void processLastFromBuffer();
    bool processLastStepByStep();
    void reset();
    bool getSkipping();
    double getCurrentTStart();
    double getCurrentTEnd();
    double getLastTStart();
    double getLastTEnd();
    unsigned int getLastIndex();
    unsigned int getCurrentSource();
    double getCurrentGeneratedTime();
    double collectDeviceTotalTime();
    double getFinalProcessTime(unsigned int index);
    double getFinalProcessTimeSquared(unsigned int index);
    double getFinalWaitingTime(unsigned int index);
    double getFinalWaitingTimeSquared(unsigned int index);
private:
    Buffer& buf;
    DeviceList& devices;
    bool isSkipping;
    unsigned int currentSourceNum;
    double currentGeneratedTime;
    unsigned int statsArraySize;
    double lastReqTime;
    double* finalProcessTime;
    double* finalProcessTimeSquared;
    double* finalWaitingTime;
    double* finalWaitingTimeSquared;
};

#endif // SELECTIONSCHEDULER_H
