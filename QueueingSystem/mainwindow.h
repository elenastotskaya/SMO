#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "statscollector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;

    SourceArray* sources;
    Buffer* buf;
    DeviceList* devices;
    BufferScheduler* bufScheduler;
    SelectionScheduler* selScheduler;
    StatsCollector* stats;

    unsigned int sourceCount;
    unsigned int bufSize;
    unsigned int deviceCount;
    double sourceMin;
    double sourceMax;
    double deviceLambda;
    bool simulationMode;

    //bool eventFilter(QObject* watched, QEvent* event);

    void resetParameters();
    void runSimulation();

private slots:
    void onButtonStartClicked();
};

#endif // MAINWINDOW_H
