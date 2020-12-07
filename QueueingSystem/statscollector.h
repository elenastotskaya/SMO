#ifndef STATSCOLLECTOR_H
#define STATSCOLLECTOR_H

#include <QVector>
#include "bufferscheduler.h"
#include "selectionscheduler.h"
#include "finalstatswindow.h"

class StatsCollector
{
public:
    StatsCollector(BufferScheduler& bufSched, SelectionScheduler& selSched, unsigned int sourceCount, unsigned int deviceCount, unsigned int bufferSize, bool mode);
    void run();
    void initNormalStep();
    bool processNormalStep();
    void initLastMinStep();
    void processLastMinStep();
    void initHangingSourceStep();
    bool processHangingSourceStep();
    bool processLastQueueStep();
    bool collectIterationStats();
    double getSourceTime(int index);
    unsigned int getGeneratedCount(int index);
    unsigned int getRefusedCount(int index);
    unsigned int getCurrentSourceIndex();
    double getCurrentSourceTime();
    unsigned int getCurrentGeneratedCount();
    unsigned int getCurrentRefusedCount();
    unsigned int getCurrentDeviceIndex();
    double getCurrentDeviceStartTime();
    double getCurrentDeviceEndTime();
    void initBufferOutput();
    unsigned int getCurrentBufferIndex();
    double getCurrentBufferTime();
    void stepBufferOutput();
private:
    BufferScheduler& bufScheduler;
    SelectionScheduler& selScheduler;
    unsigned int sourceCount;
    unsigned int deviceCount;
    unsigned int bufferSize;
    bool simulationMode;
    unsigned int maxRequestCount;
    static constexpr double t = 1.643;
    static constexpr double delta = 0.1;
    static constexpr unsigned int MAX_N = 100000;
    static constexpr double MIN_P = 1e-5;
    static constexpr unsigned int MAX_REQ_COUNT_INIT = 100;
    static constexpr double DUMMY_PROB = -1.0;
    double pPrev;
    bool precise;
    unsigned int generatedSum;
    unsigned int tempIndexCount;
    QVector<unsigned int> generatedCount;
    QVector<double> probRefusal;
    QVector<double> avgSystem;
    QVector<double> avgWaiting;
    QVector<double> avgProcessing;
    QVector<double> dispWaiting;
    QVector<double> dispProcessing;
    QVector<double> operatingRatio;
    FinalStatsWindow* finalStats;

    void collect();
    void collectFromDevices();
    void innerLoop();
    void display();
};

#endif // STATSCOLLECTOR_H
