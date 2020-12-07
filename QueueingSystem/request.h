#ifndef REQUEST_H
#define REQUEST_H


class Request
{
public:
    Request(double time = DUMMY_TIME, unsigned int num = 0);
    bool isEmpty();
    double getTime();
    unsigned int getNum();
private:
    double issueTime;
    unsigned int sourceNum;
    static constexpr double DUMMY_TIME = -1.0; //should be less than 0
};

#endif // REQUEST_H
