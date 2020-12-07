#include "source.h"
#include <chrono>

Source::Source(double tauMin, double tauMax)
    :generator(std::random_device()()),
    uniformDist(tauMin, tauMax),
    currTime(0.0),
    generatedCount(0)
{
}

void Source::generateRequest()
{
    currTime += uniformDist(generator);
    ++generatedCount;
}

double Source::getTime()
{
    return currTime;
}

unsigned int Source::getGeneratedCount()
{
    return generatedCount;
}
