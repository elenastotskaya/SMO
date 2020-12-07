#include "stepbystepwindow.h"
#include "ui_stepbystepwindow.h"
#include <algorithm>
#include <initializer_list>

StepByStepWindow::StepByStepWindow(StatsCollector& statsCollector, unsigned int inputSourceCount,
        unsigned int inputBufferSize, unsigned int inputDeviceCount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepByStepWindow),
    stats(statsCollector),
    sourceCount(inputSourceCount),
    bufferSize(inputBufferSize),
    deviceCount(inputDeviceCount),
    isGenerationComplete(false),
    isLastMinProcessed(false),
    isHangingSourcesProcessed(false),
    isLastQueueStepsProcessed(false),
    isSimulationFinished(false),
    queueStep(true)
{
    ui->setupUi(this);
    setWindowTitle(tr("Current state"));
    connect(ui->buttonNext, SIGNAL(clicked()), this, SLOT(onButtonNextClicked()));
    ui->tableSources->setRowCount(sourceCount);
    ui->tableSources->setColumnCount(SOURCE_PARAM_COUNT);
    unsigned int sourceTableWidth = NUMBER_CELL_WIDTH + SOURCE_PARAM_COUNT * CELL_WIDTH;
    unsigned int sourceTableHeight = HEADER_CELL_HEIGHT + sourceCount * CELL_HEIGHT;
    ui->tableSources->setGeometry(0, 0, sourceTableWidth, sourceTableHeight);
    ui->tableSources->horizontalHeader()->setDefaultSectionSize(CELL_WIDTH);
    ui->tableSources->verticalHeader()->setDefaultSectionSize(CELL_HEIGHT);
    sourceHeaders << "Current time" << "Generated request count" << "Refused request count";
    ui->tableSources->setHorizontalHeaderLabels(sourceHeaders);
    for (unsigned int i = 0; i < sourceCount; ++i) {
        sourceIndexes << QString::number(i);
    }
    ui->tableSources->setVerticalHeaderLabels(sourceIndexes);

    ui->tableBuffer->setRowCount(bufferSize);
    ui->tableBuffer->setColumnCount(BUFFER_PARAM_COUNT);
    unsigned int bufferTableWidth = NUMBER_CELL_WIDTH + BUFFER_PARAM_COUNT * CELL_WIDTH;
    unsigned int bufferTableHeight = HEADER_CELL_HEIGHT + bufferSize * CELL_HEIGHT;
    ui->tableBuffer->setGeometry(sourceTableWidth + MARGIN, 0, bufferTableWidth, bufferTableHeight);
    ui->tableBuffer->horizontalHeader()->setDefaultSectionSize(CELL_WIDTH);
    ui->tableBuffer->verticalHeader()->setDefaultSectionSize(CELL_HEIGHT);
    bufferHeaders << "Request time" << "Request source";
    ui->tableBuffer->setHorizontalHeaderLabels(bufferHeaders);
    for (unsigned int i = 0; i < bufferSize; ++i) {
        bufferIndexes << QString::number(i);
    }
    ui->tableBuffer->setVerticalHeaderLabels(bufferIndexes);

    ui->tableDevices->setRowCount(deviceCount);
    ui->tableDevices->setColumnCount(DEVICE_PARAM_COUNT);
    unsigned int deviceTableWidth = NUMBER_CELL_WIDTH + DEVICE_PARAM_COUNT * CELL_WIDTH;
    unsigned int deviceTableHeight = HEADER_CELL_HEIGHT + deviceCount * CELL_HEIGHT;
    ui->tableDevices->setGeometry(sourceTableWidth + bufferTableWidth + 2 * MARGIN, 0, deviceTableWidth, deviceTableHeight);
    ui->tableDevices->horizontalHeader()->setDefaultSectionSize(CELL_WIDTH);
    ui->tableDevices->verticalHeader()->setDefaultSectionSize(CELL_HEIGHT);
    deviceHeaders << "Next index" << "Current start time" << "Current end time";
    ui->tableDevices->setHorizontalHeaderLabels(deviceHeaders);
    for (unsigned int i = 0; i < deviceCount; ++i) {
        deviceIndexes << QString::number(i);
    }
    ui->tableDevices->setVerticalHeaderLabels(deviceIndexes);

    unsigned int maxHeight = std::max({sourceTableHeight, bufferTableHeight, deviceTableHeight});
    ui->buttonBox->setGeometry(sourceTableWidth + bufferTableWidth + deviceTableWidth - BUTTON_BOX_WIDTH,
        maxHeight + MARGIN, BUTTON_BOX_WIDTH, BUTTON_BOX_HEIGHT);
    ui->buttonNext->setGeometry(sourceTableWidth + bufferTableWidth + deviceTableWidth - BUTTON_BOX_WIDTH - BUTTON_WIDTH,
        maxHeight + 2 * MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT);
    ui->textEditEvent->setGeometry(sourceTableWidth + bufferTableWidth + deviceTableWidth - 2*BUTTON_BOX_WIDTH - TEXT_EDIT_WIDTH,
        maxHeight + MARGIN, TEXT_EDIT_WIDTH, BUTTON_BOX_HEIGHT);
    ui->textEditEvent->setReadOnly(true);

    resize(sourceTableWidth + bufferTableWidth + deviceTableWidth + 2 * MARGIN, maxHeight + BUTTON_BOX_HEIGHT + 2 * MARGIN);
    initTables();
}

StepByStepWindow::~StepByStepWindow()
{
    delete ui;
}

void StepByStepWindow::initTables()
{
    for (unsigned int i = 0; i < sourceCount; ++i) {
        ui->tableSources->setItem(i, 0, new QTableWidgetItem(QString::number(stats.getSourceTime(i))));
        ui->tableSources->setItem(i, 1, new QTableWidgetItem(QString::number(stats.getGeneratedCount(i))));
        ui->tableSources->setItem(i, 2, new QTableWidgetItem(QString::number(stats.getRefusedCount(i))));
    }
    for (unsigned int i = 0; i < bufferSize; ++i) {
        for (unsigned int j = 0; j < BUFFER_PARAM_COUNT; ++j) {
            ui->tableBuffer->setItem(i, j, new QTableWidgetItem("Empty"));
        }
    }
    for (unsigned int i = 0; i < deviceCount; ++i) {
        ui->tableDevices->setItem(i, 0, new QTableWidgetItem(""));
        for (unsigned int j = 1; j < DEVICE_PARAM_COUNT; ++j) {
            ui->tableDevices->setItem(i, j, new QTableWidgetItem("0"));
        }
    }
    currentDeviceIndex = stats.getCurrentDeviceIndex();
    if (currentDeviceIndex < deviceCount - 1) {
        ui->tableDevices->setItem(currentDeviceIndex + 1, 0, new QTableWidgetItem("*"));
    } else {
        ui->tableDevices->setItem(0, 0, new QTableWidgetItem("*"));
    }
}

void StepByStepWindow::onButtonNextClicked()
{
    if (!isSimulationFinished) {
        if (!isGenerationComplete) {
            if (queueStep) {
                stats.initNormalStep();
                queueStep = false;
            } else {
                if (stats.processNormalStep()) {
                    isGenerationComplete = true;
                }
                queueStep = true;
            }
        } else {
            if (!isLastMinProcessed) {
                if (queueStep) {
                    stats.initLastMinStep();
                    queueStep = false;

                } else {
                    stats.processLastMinStep();
                    queueStep = true;
                    isLastMinProcessed = true;
                }
            } else {
                if (!isHangingSourcesProcessed) {
                    if (queueStep) {
                        stats.initHangingSourceStep();
                        queueStep = false;
                    } else {
                        if(stats.processHangingSourceStep()) {
                            isHangingSourcesProcessed = true;
                        }
                        queueStep = true;
                    }
                } else {
                    if (!isLastQueueStepsProcessed) {
                        if(stats.processLastQueueStep()) {
                            isLastQueueStepsProcessed = true;
                        }
                    } else {
                        if (stats.collectIterationStats()) {
                            isSimulationFinished = true;
                        }
                        isGenerationComplete = false;
                        isLastMinProcessed = false;
                        isHangingSourcesProcessed = false;
                        isLastQueueStepsProcessed = false;
                        initTables();
                    }
                }
            }
        }
        ui->textEditEvent->clear();
        if (!queueStep) {
            if (ui->tableBuffer->item(bufferSize - 1, 0)->text() == "Empty") {
                ui->textEditEvent->insertPlainText("Request added to buffer");
            } else {
                ui->textEditEvent->insertPlainText("Request added to buffer, replacing last request");
            }
            prevSourceIndex = currentSourceIndex;
            currentSourceIndex = stats.getCurrentSourceIndex();
            ui->tableSources->setItem(currentSourceIndex, 0, new QTableWidgetItem(QString::number(stats.getCurrentSourceTime())));
            ui->tableSources->setItem(currentSourceIndex, 1, new QTableWidgetItem(QString::number(stats.getCurrentGeneratedCount())));
            ui->tableSources->setItem(prevSourceIndex, 2, new QTableWidgetItem(QString::number(stats.getCurrentRefusedCount())));
            stats.initBufferOutput();
            double bufTime;
            for (unsigned int i = 0; i < bufferSize; ++i) {
                bufTime = stats.getCurrentBufferTime();
                if (bufTime >= 0) {
                    ui->tableBuffer->setItem(i, 0, new QTableWidgetItem(QString::number(bufTime)));
                    ui->tableBuffer->setItem(i, 1, new QTableWidgetItem(QString::number(stats.getCurrentBufferIndex())));
                } else {
                    ui->tableBuffer->setItem(i, 0, new QTableWidgetItem("Empty"));
                    ui->tableBuffer->setItem(i, 1, new QTableWidgetItem("Empty"));
                }
                stats.stepBufferOutput();
            }
        } else {
            stats.initBufferOutput();
            double bufTime;
            for (unsigned int i = 0; i < bufferSize; ++i) {
                bufTime = stats.getCurrentBufferTime();
                if (bufTime >= 0) {
                    ui->tableBuffer->setItem(i, 0, new QTableWidgetItem(QString::number(bufTime)));
                    ui->tableBuffer->setItem(i, 1, new QTableWidgetItem(QString::number(stats.getCurrentBufferIndex())));
                } else {
                    ui->tableBuffer->setItem(i, 0, new QTableWidgetItem("Empty"));
                    ui->tableBuffer->setItem(i, 1, new QTableWidgetItem("Empty"));
                }
                stats.stepBufferOutput();
            }
            if (currentDeviceIndex < deviceCount - 1) {
                ui->tableDevices->setItem(currentDeviceIndex + 1, 0, new QTableWidgetItem(""));
            } else {
                ui->tableDevices->setItem(0, 0, new QTableWidgetItem(""));
            }
            currentDeviceIndex = stats.getCurrentDeviceIndex();
            if (currentDeviceIndex < deviceCount - 1) {
                ui->tableDevices->setItem(currentDeviceIndex + 1, 0, new QTableWidgetItem("*"));
            } else {
                ui->tableDevices->setItem(0, 0, new QTableWidgetItem("*"));
            }
            QString prevTStart = ui->tableDevices->item(currentDeviceIndex, 1)->text();
            ui->tableDevices->setItem(currentDeviceIndex, 1, new QTableWidgetItem(QString::number(stats.getCurrentDeviceStartTime())));
            ui->tableDevices->setItem(currentDeviceIndex, 2, new QTableWidgetItem(QString::number(stats.getCurrentDeviceEndTime())));
            if (ui->tableDevices->item(currentDeviceIndex, 1)->text() == prevTStart) {
                ui->textEditEvent->insertPlainText("No idle devices");
            } else {
                ui->textEditEvent->insertPlainText("Request set to device");
            }
        }
    } else {
        done(0);
    }
}
