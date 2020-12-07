#include "sourcearray.h"

SourceArray::SourceArray(unsigned int size, double min, double max)
    :arraySize(size),
    commonMin(min),
    commonMax(max),
    minIndex(DUMMY_MIN_INDEX),
    prevMinIndex(DUMMY_MIN_INDEX)
{
    block = operator new[](size * sizeof(Source));
    sources = static_cast<Source*>(block);
    for (unsigned int i = 0; i < arraySize; ++i) {
        new(&sources[i]) Source(commonMin, commonMax);
    }
    initSources();
}

SourceArray::~SourceArray()
{
    for (unsigned int i = arraySize; i > 0; --i) {
        sources[i-1].~Source();
    }
    operator delete[](block);
}

double SourceArray::chooseMin()
{
    prevMinIndex = minIndex;
    double min = sources[0].getTime();
    minIndex = 0;
    for (unsigned int i = 1; i < arraySize; ++i) {
        if (sources[i].getTime() < min) {
            min = sources[i].getTime();
            minIndex = i;
        }
    }
    if (prevMinIndex == DUMMY_MIN_INDEX) {
        prevMinIndex = minIndex;
    }
    return min;
}

double SourceArray::chooseMinAndGenerate()
{
    double min = chooseMin();
    sources[minIndex].generateRequest();
    return min;
}

double SourceArray::chooseRemainingMin()
{
    prevMinIndex = minIndex;
    unsigned int tempIndex = 0;
    while (sources[tempIndex].getTime() <= sources[minIndex].getTime()) {
        ++tempIndex;
    }
    double min = sources[tempIndex].getTime();
    unsigned int tempNext = tempIndex + 1;
    for (unsigned int i = tempNext; i < arraySize; ++i) {
        if (sources[i].getTime() < min && sources[i].getTime() > sources[minIndex].getTime()) {
            min = sources[i].getTime();
            tempIndex = i;
        }
    }
    minIndex = tempIndex;
    return min;
}

void SourceArray::initSources()
{
    for (unsigned int i = 0; i < arraySize; ++i) {
        sources[i].generateRequest();
    }
    minInitialTime = sources[0].getTime();
    for (unsigned int i = 1; i < arraySize; ++i) {
        if (sources[i].getTime() < minInitialTime) {
            minInitialTime = sources[i].getTime();
        }
    }
}

void SourceArray::reset()
{
    for (unsigned int i = 0; i < arraySize; ++i) {
        sources[i] = Source(commonMin, commonMax);
    }
    initSources();
    prevMinIndex = DUMMY_MIN_INDEX;
}

unsigned int SourceArray::getSize()
{
    return arraySize;
}

unsigned int SourceArray::getMinIndex()
{
    return minIndex;
}

unsigned int SourceArray::getPrevMinIndex()
{
    return prevMinIndex;
}

double SourceArray::getMinInitial()
{
    return minInitialTime;
}

unsigned int SourceArray::getGenerated(unsigned int index)
{
    return sources[index].getGeneratedCount();
}

double SourceArray::getGeneratedTime(unsigned int index)
{
    return sources[index].getTime();
}

unsigned int SourceArray::getCurrentGenerated()
{
    return sources[minIndex].getGeneratedCount();
}

double SourceArray::getCurrentGeneratedTime()
{
    return sources[minIndex].getTime();
}
