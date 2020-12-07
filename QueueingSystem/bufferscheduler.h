#ifndef BUFFERSCHEDULER_H
#define BUFFERSCHEDULER_H

#include "request.h"
#include "buffer.h"
#include "sourcearray.h"

class BufferScheduler
{
public:
    BufferScheduler(SourceArray& inputArray, Buffer& outputBuf);
    ~BufferScheduler();
    void processRequest(unsigned int selectionMode);
    void reset();
    unsigned int getGenerated(unsigned int index);
    unsigned int getRefused(unsigned int index);
    double getGeneratedTime(unsigned int index);
    unsigned int getCurrentSourceIndex();
    unsigned int getCurrentGenerated();
    unsigned int getCurrentRefused();
    double getCurrentGeneratedTime();
    void initBufferOutput();
    unsigned int getCurrentBufferIndex();
    double getCurrentBufferTime();
    void stepBufferOutput();
    double getMinInitial();
private:
    SourceArray& sources;
    Buffer& buf;
    unsigned int* generatedCount;
    unsigned int* refusedCount;
    double* generatedTime;
};

#endif // BUFFERSCHEDULER_H
