#include "device.h"

Device::Device(double lambda)
    :generator(std::random_device()()),
    expDist(lambda),
    processedCount(0),
    tStart(0.0),
    tEnd(0.0),
    totalTime(0.0)
{
}

void Device::processRequest(Request req)
{
    if (req.getTime() < tEnd) {
        tStart = tEnd;
        tEnd += expDist(generator);
    } else {
        tStart = req.getTime();
        tEnd = tStart + expDist(generator);
    }
    totalTime += (tEnd - tStart);
}

void Device::reset()
{
    processedCount = 0;
    totalTime = 0.0;
    tStart = 0.0;
    tEnd = 0.0;
}

double Device::getTStart()
{
    return tStart;
}

double Device::getTEnd()
{
    return tEnd;
}

double Device::getTotalTime()
{
    return totalTime;
}
