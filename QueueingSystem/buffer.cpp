#include "buffer.h"

Buffer::Buffer(unsigned int bufSize)
 :size(bufSize)
{
    head = new BufferEl;
    tail = head;
    for (unsigned int i = 1; i < bufSize; ++i) {
        tail->next = new BufferEl;
        tail = tail->next;
    }
    tail->next = head;
}

Buffer::~Buffer()
{
    tail = head->next;
    head->next = nullptr;
    while(tail->next != nullptr) {
        head = tail->next;
        delete tail;
        tail = head;
    }
    delete tail;
}

bool Buffer::isFull()
{
    return tail->next == head && !head->content.isEmpty();
}

bool Buffer::add(Request newEl)
{
    if (!isFull()) {
        tail = tail->next;
        tail->content = newEl;
        return true;
    } else {
        tail->content = newEl;
        return false;
    }
}

Request Buffer::release()
{
    Request firstReq = head->content;
    head->content = Request();
    head = head->next;
    return firstReq;
}

void Buffer::reset()
{
    head->content = Request();
    tail = head;
    while (head->next != tail) {
        head = head->next;
        head->content = Request();
    }
    head = tail->next;
}

void Buffer::initOutput()
{
    outputPtr = head;
}

unsigned int Buffer::getCurrentIndex()
{
    return outputPtr->content.getNum();
}

double Buffer::getCurrentTime()
{
    return outputPtr->content.getTime();
}

void Buffer::stepOutput()
{
    outputPtr = outputPtr->next;
}

double Buffer::getHeadTime()
{
    return head->content.getTime();
}

unsigned int Buffer::getHeadSource()
{
    return head->content.getNum();
}

double Buffer::getTailTime()
{
    return tail->content.getTime();
}

unsigned int Buffer::getSize()
{
    return size;
}
