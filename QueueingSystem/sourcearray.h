#ifndef SOURCEARRAY_H
#define SOURCEARRAY_H

#include "source.h"
#include <limits>

class SourceArray
{
public:
    SourceArray(unsigned int size, double min, double max);
    ~SourceArray();
    double chooseMin();
    double chooseMinAndGenerate();
    double chooseRemainingMin();
    void initSources();
    void reset();
    unsigned int getSize();
    unsigned int getMinIndex();
    unsigned int getPrevMinIndex();
    double getMinInitial();
    unsigned int getGenerated(unsigned int index);
    double getGeneratedTime(unsigned int index);
    unsigned int getCurrentGenerated();
    double getCurrentGeneratedTime();
private:
    unsigned int arraySize;
    Source* sources;
    double commonMin;
    double commonMax;
    unsigned int minIndex;
    unsigned int prevMinIndex;
    static constexpr unsigned int DUMMY_MIN_INDEX = std::numeric_limits<unsigned int>::max();
    double minInitialTime;
    void* block;
};

#endif // SOURCEARRAY_H
