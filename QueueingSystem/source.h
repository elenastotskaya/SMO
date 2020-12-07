#ifndef SOURCE_H
#define SOURCE_H

#include <random>

class Source
{
public:
  Source(double tauMin, double tauMax);
  void generateRequest();
  double getTime();
  unsigned int getGeneratedCount();
private:
  std::mt19937 generator;
  std::uniform_real_distribution<> uniformDist;
  double currTime;
  unsigned int generatedCount;
};

#endif // SOURCE_H
