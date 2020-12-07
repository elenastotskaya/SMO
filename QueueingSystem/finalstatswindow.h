#ifndef FINALSTATSWINDOW_H
#define FINALSTATSWINDOW_H

#include <QDialog>
#include <QVector>

namespace Ui {
class FinalStatsWindow;
}

class FinalStatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FinalStatsWindow(unsigned int sourceCount, unsigned int deviceCount,
        QVector<unsigned int>& generatedCount, QVector<double>& probRefusal,
        QVector<double>& avgSystem, QVector<double>& avgWaiting, QVector<double>& avgProcessing,
        QVector<double>& dispWaiting, QVector<double>& dispProcessing, QVector<double>& operatingRatio,
        QWidget *parent = nullptr);
    ~FinalStatsWindow();

private:
    Ui::FinalStatsWindow *ui;
    QStringList sourceHeaders;
    QStringList sourceIndexes;
    QStringList deviceHeaders;
    QStringList deviceIndexes;
    static constexpr unsigned int CELL_WIDTH = 250;
    static constexpr unsigned int CELL_HEIGHT = 40;
    static constexpr unsigned int NUMBER_CELL_WIDTH = 50;
    static constexpr unsigned int HEADER_CELL_HEIGHT = 40;
    static constexpr unsigned int SOURCE_PARAM_COUNT = 7;
    static constexpr unsigned int DEVICE_PARAM_COUNT = 1;
    static constexpr unsigned int MARGIN = 10;
    static constexpr unsigned int BUTTON_VERTICAL_MARGIN = 50;
    static constexpr unsigned int BUTTON_HORIZONTAL_MARGIN = 200;
    static constexpr unsigned int SOURCE_TABLE_WIDTH = NUMBER_CELL_WIDTH + SOURCE_PARAM_COUNT * CELL_WIDTH;
    static constexpr unsigned int DEVICE_TABLE_WIDTH = NUMBER_CELL_WIDTH + DEVICE_PARAM_COUNT * CELL_WIDTH;
};

#endif // FINALSTATSWINDOW_H
