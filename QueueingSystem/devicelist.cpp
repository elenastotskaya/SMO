#include "devicelist.h"

DeviceEl::DeviceEl(double contentLambda, unsigned int currentIndex)
    :content(contentLambda),
    index(currentIndex),
    next(nullptr)
{
}

DeviceList::DeviceList(unsigned int listSize, double lambda)
    : size(listSize),
    globalLambda(lambda)
{
    current = new DeviceEl(lambda, 0);
    last = current;
    for (unsigned int i = 1; i < listSize; ++i) {
        last->next = new DeviceEl(lambda, i);
        last = last->next;
    }
    last->next = current;
}

DeviceList::~DeviceList()
{
    last = current->next;
    current->next = nullptr;
    while(last->next != nullptr) {
        current = last->next;
        delete last;
        last = current;
    }
    delete last;
}

bool DeviceList::isLooped()
{
    return last->next == current;
}

void DeviceList::stepCurrent()
{
    current = current->next;
}

void DeviceList::addRequest(Request req)
{
    current->content.processRequest(req);
    last = current;
    current = last->next;
}

void DeviceList::addRequestByMinTime(Request req)
{
    double minTEnd = current->content.getTEnd();
    DeviceEl* minDevice = current;
    current = current->next;
    while(!isLooped())
    {
        if (current->content.getTEnd() < minTEnd) {
            minTEnd = current->content.getTEnd();
            minDevice = current;
        }
        current = current->next;
    }
    current = minDevice;
    addRequest(req);
}

void DeviceList::reset()
{
    current->content.reset();
    last = current;
    for (unsigned int i = 1; i < size; ++i) {
        last->next->content.reset();
        last = last->next;
    }
    last->next = current;
}

unsigned int DeviceList::getSize()
{
    return size;
}

double DeviceList::getCurrentTStart()
{
    return current->content.getTStart();
}

double DeviceList::getCurrentTEnd()
{
    return current->content.getTEnd();
}

double DeviceList::getLastTStart()
{
    return last->content.getTStart();
}

double DeviceList::getLastTEnd()
{
    return last->content.getTEnd();
}

unsigned int DeviceList::getLastIndex()
{
    return last->index;
}

double DeviceList::getCurrentTotalTime()
{
    return current->content.getTotalTime();
}
