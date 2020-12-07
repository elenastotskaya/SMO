#include "request.h"

Request::Request(double time, unsigned int num)
    :issueTime(time),
    sourceNum(num)
{
}

bool Request::isEmpty()
{
    return issueTime < 0;
}

double Request::getTime()
{
    return issueTime;
}

unsigned int Request::getNum()
{
    return sourceNum;
}
