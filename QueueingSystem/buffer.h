#ifndef BUFFER_H
#define BUFFER_H

#include "request.h"

struct BufferEl
{
  Request content;
  BufferEl* next;
};

class Buffer
{
public:
    Buffer(unsigned int bufSize);
    ~Buffer();
    bool add(Request newEl);
    Request release();
    void reset();
    void initOutput();
    unsigned getCurrentIndex();
    double getCurrentTime();
    void stepOutput();
    double getHeadTime();
    unsigned int getHeadSource();
    double getTailTime();
    unsigned int getSize();
private:
    bool isFull();
    BufferEl* head;
    BufferEl* tail;
    BufferEl* outputPtr;
    unsigned int size;
};

#endif // BUFFER_H
