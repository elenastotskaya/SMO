#include "finalstatswindow.h"
#include "ui_finalstatswindow.h"
#include <algorithm>

FinalStatsWindow::FinalStatsWindow(unsigned int sourceCount, unsigned int deviceCount,
        QVector<unsigned int>& generatedCount, QVector<double>& probRefusal,
        QVector<double>& avgSystem, QVector<double>& avgWaiting, QVector<double>& avgProcessing,
        QVector<double>& dispWaiting, QVector<double>& dispProcessing, QVector<double>& operatingRatio,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinalStatsWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Simulation results"));
    ui->tableSources->setRowCount(sourceCount);
    ui->tableSources->setColumnCount(SOURCE_PARAM_COUNT);
    ui->tableSources->setGeometry(0, 0, SOURCE_TABLE_WIDTH, HEADER_CELL_HEIGHT + sourceCount * CELL_HEIGHT);
    ui->tableSources->horizontalHeader()->setDefaultSectionSize(CELL_WIDTH);
    ui->tableSources->verticalHeader()->setDefaultSectionSize(CELL_HEIGHT);

    ui->tableDevices->setRowCount(deviceCount);
    ui->tableDevices->setColumnCount(DEVICE_PARAM_COUNT);
    ui->tableDevices->setGeometry(0, HEADER_CELL_HEIGHT + sourceCount * CELL_HEIGHT + MARGIN, DEVICE_TABLE_WIDTH, HEADER_CELL_HEIGHT + deviceCount * CELL_HEIGHT);
    ui->tableDevices->horizontalHeader()->setDefaultSectionSize(CELL_WIDTH);
    ui->tableDevices->verticalHeader()->setDefaultSectionSize(CELL_HEIGHT);

    sourceHeaders << "Generated request count" << "Probability of refusal" << "Average system time" << "Average waiting time"
        << "Average processing time" << "Waiting time variance" << "Processing time variance";
    ui->tableSources->setHorizontalHeaderLabels(sourceHeaders);

    deviceHeaders << "Operating ratio";
    ui->tableDevices->setHorizontalHeaderLabels(deviceHeaders);

    for (unsigned int i = 0; i < sourceCount; ++i) {
        sourceIndexes << QString::number(i);
        ui->tableSources->setItem(i, 0, new QTableWidgetItem(QString::number(generatedCount[i])));
        ui->tableSources->setItem(i, 1, new QTableWidgetItem(QString::number(probRefusal[i])));
        ui->tableSources->setItem(i, 2, new QTableWidgetItem(QString::number(avgSystem[i])));
        ui->tableSources->setItem(i, 3, new QTableWidgetItem(QString::number(avgWaiting[i])));
        ui->tableSources->setItem(i, 4, new QTableWidgetItem(QString::number(avgProcessing[i])));
        ui->tableSources->setItem(i, 5, new QTableWidgetItem(QString::number(dispWaiting[i])));
        ui->tableSources->setItem(i, 6, new QTableWidgetItem(QString::number(dispProcessing[i])));
    }
    ui->tableSources->setVerticalHeaderLabels(sourceIndexes);

    for (unsigned int i = 0; i < deviceCount; ++i) {
        deviceIndexes << QString::number(i);
        ui->tableDevices->setItem(i, 0, new QTableWidgetItem(QString::number(operatingRatio[i])));
    }
    ui->tableDevices->setVerticalHeaderLabels(deviceIndexes);
    unsigned int maxWidth = (SOURCE_TABLE_WIDTH > DEVICE_TABLE_WIDTH) ? SOURCE_TABLE_WIDTH : DEVICE_TABLE_WIDTH;
    ui->buttonBox->setGeometry(maxWidth - BUTTON_HORIZONTAL_MARGIN - MARGIN, HEADER_CELL_HEIGHT*2 + sourceCount * CELL_HEIGHT + deviceCount * CELL_HEIGHT + MARGIN,
        BUTTON_HORIZONTAL_MARGIN, BUTTON_VERTICAL_MARGIN);
    resize(maxWidth, HEADER_CELL_HEIGHT*2 + sourceCount * CELL_HEIGHT + deviceCount * CELL_HEIGHT + MARGIN + BUTTON_VERTICAL_MARGIN);
}

FinalStatsWindow::~FinalStatsWindow()
{
    delete ui;
}
