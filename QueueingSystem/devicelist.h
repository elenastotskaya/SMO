#ifndef DEVICELIST_H
#define DEVICELIST_H

#include "device.h"

struct DeviceEl
{
    Device content;
    unsigned int index;
    DeviceEl* next;
    DeviceEl(double contentLambda, unsigned int currentIndex);
};

class DeviceList
{
public:
    DeviceList(unsigned int listSize, double globalLambda);
    ~DeviceList();
    bool isLooped();
    void stepCurrent();
    void addRequest(Request req);
    void addRequestByMinTime(Request req);
    void reset();
    unsigned int getSize();
    double getCurrentTStart();
    double getCurrentTEnd();
    double getLastTStart();
    double getLastTEnd();
    unsigned int getLastIndex();
    double getCurrentTotalTime();
private:
    DeviceEl* current;
    DeviceEl* last;
    unsigned int size;
    double globalLambda;
};

#endif // DEVICELIST_H
