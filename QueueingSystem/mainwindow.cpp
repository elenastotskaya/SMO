#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sourceCount(0),
    bufSize(0),
    deviceCount(0),
    sourceMin(-1.0),
    sourceMax(-1.0),
    deviceLambda(-1.0),
    simulationMode(false)
{
    ui->setupUi(this);
    setWindowTitle(tr("Queueing System Model"));
    ui->buttonAutomatic->setChecked(true);
    connect(ui->buttonStart, SIGNAL(clicked()), this, SLOT(onButtonStartClicked()));
    ui->inSourceCount->setClearButtonEnabled(true);
    ui->inBufSize->setClearButtonEnabled(true);
    ui->inDeviceCount->setClearButtonEnabled(true);
    ui->inSourceMin->setClearButtonEnabled(true);
    ui->inSourceMax->setClearButtonEnabled(true);
    ui->inDeviceLambda->setClearButtonEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonStartClicked()
{
    int tempCount;
    bool isCorrect = true;
    try {
        tempCount = std::stoi((ui->inSourceCount->text()).toStdString());
        if (tempCount < 1) {
            ui->inSourceCount->setText("Number should be positive");
            isCorrect = false;
        } else {
            sourceCount = static_cast<unsigned int>(tempCount);
        }
    } catch(std::invalid_argument) {
        ui->inSourceCount->setText("Invalid input");
        isCorrect = false;
    } catch(std::out_of_range) {
        ui->inSourceCount->setText("Number out of range");
        isCorrect = false;
    }

    try {
        tempCount = std::stoi((ui->inBufSize->text()).toStdString());
        if (tempCount < 1) {
            ui->inBufSize->setText("Number should be positive");
            isCorrect = false;
        } else {
            bufSize = static_cast<unsigned int>(tempCount);
        }
    } catch(std::invalid_argument) {
        ui->inBufSize->setText("Invalid input");
        isCorrect = false;
    } catch(std::out_of_range) {
        ui->inBufSize->setText("Number out of range");
        isCorrect = false;
    }

    try {
        tempCount = std::stoi((ui->inDeviceCount->text()).toStdString());
        if (tempCount < 1) {
            ui->inDeviceCount->setText("Number should be positive");
            isCorrect = false;
        } else {
            deviceCount = static_cast<unsigned int>(tempCount);
        }
    } catch(std::invalid_argument) {
        ui->inDeviceCount->setText("Invalid input");
        isCorrect = false;
    } catch(std::out_of_range) {
        ui->inDeviceCount->setText("Number out of range");
        isCorrect = false;
    }

    double tempMul;
    try {
        tempMul = std::stod((ui->inSourceMin->text()).toStdString());
        if (tempMul < 0) {
            ui->inSourceMin->setText("Time cannot be negative");
            isCorrect = false;
        } else {
            sourceMin = tempMul;
        }
    } catch(std::invalid_argument) {
        ui->inSourceMin->setText("Invalid input");
        isCorrect = false;
    } catch(std::out_of_range) {
        ui->inSourceMin->setText("Number out of range");
        isCorrect = false;
    }

    try {
        tempMul = std::stod((ui->inSourceMax->text()).toStdString());
        if (tempMul < 0) {
            ui->inSourceMax->setText("Time cannot be negative");
            isCorrect = false;
        } else if (tempMul <= sourceMin) {
            ui->inSourceMax->setText("Less than minimum time");
            isCorrect = false;
        } else {
            sourceMax = tempMul;
        }
    } catch(std::invalid_argument) {
        ui->inSourceMax->setText("Invalid input");
        isCorrect = false;
    } catch(std::out_of_range) {
        ui->inSourceMax->setText("Number out of range");
        isCorrect = false;
    }

    try {
        tempMul = std::stod((ui->inDeviceLambda->text()).toStdString());
        if (tempMul <= 0) {
            ui->inDeviceLambda->setText("Lambda should be positive");
            isCorrect = false;
        } else {
            deviceLambda = tempMul;
        }
    } catch(std::invalid_argument) {
        ui->inDeviceLambda->setText("Invalid input");
        isCorrect = false;
    } catch(std::out_of_range) {
        ui->inDeviceLambda->setText("Number out of range");
        isCorrect = false;
    }

    if (ui->buttonStep->isChecked()) {
        simulationMode = true;
    }

    if (isCorrect) {
        runSimulation();
    }

    resetParameters();
}

void MainWindow::resetParameters()
{
    sourceCount = 0;
    bufSize = 0;
    deviceCount = 0;
    sourceMin = -1.0;
    sourceMax = -1.0;
    deviceLambda = -1.0;
    simulationMode = false;
}

void MainWindow::runSimulation()
{
    sources = new SourceArray(sourceCount, sourceMin, sourceMax);
    buf = new Buffer(bufSize);
    devices = new DeviceList(deviceCount, deviceLambda);
    bufScheduler = new BufferScheduler(*sources, *buf);
    selScheduler = new SelectionScheduler(*buf, *devices, sourceCount);
    stats = new StatsCollector(*bufScheduler, *selScheduler, sourceCount, deviceCount, bufSize, simulationMode);
    stats->run();
    delete stats;
    delete selScheduler;
    delete bufScheduler;
    delete devices;
    delete buf;
    delete sources;
}
