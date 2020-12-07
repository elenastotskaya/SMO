#ifndef DEVICE_H
#define DEVICE_H

#include "request.h"
#include <random>

class Device
{
public:
  Device(double lambda = DEFAULT_LAMBDA);
  void processRequest(Request req);
  void reset();
  double getTStart();
  double getTEnd();
  double getTotalTime();
private:
  std::mt19937 generator;
  std::exponential_distribution<double> expDist;
  unsigned int processedCount;
  double tStart;
  double tEnd;
  double totalTime;
  static constexpr double DEFAULT_LAMBDA = 1.0;
};

#endif // DEVICE_H
