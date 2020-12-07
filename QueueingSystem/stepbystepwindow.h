#ifndef STEPBYSTEPWINDOW_H
#define STEPBYSTEPWINDOW_H

#include <QDialog>
#include "statscollector.h"

namespace Ui {
class StepByStepWindow;
}

class StepByStepWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StepByStepWindow(StatsCollector& statsCollector, unsigned int inputSourceCount,
        unsigned int inputBufferSize, unsigned int inputDeviceCount, QWidget *parent = nullptr);
    ~StepByStepWindow();

private:
    Ui::StepByStepWindow *ui;
    StatsCollector& stats;
    unsigned int sourceCount;
    unsigned int bufferSize;
    unsigned int deviceCount;
    QStringList sourceHeaders;
    QStringList sourceIndexes;
    QStringList bufferHeaders;
    QStringList bufferIndexes;
    QStringList deviceHeaders;
    QStringList deviceIndexes;
    static constexpr unsigned int CELL_WIDTH = 210;
    static constexpr unsigned int CELL_HEIGHT = 40;
    static constexpr unsigned int NUMBER_CELL_WIDTH = 50;
    static constexpr unsigned int HEADER_CELL_HEIGHT = 40;
    static constexpr unsigned int SOURCE_PARAM_COUNT = 3;
    static constexpr unsigned int BUFFER_PARAM_COUNT = 2;
    static constexpr unsigned int DEVICE_PARAM_COUNT = 3;
    static constexpr unsigned int MARGIN = 10;
    static constexpr unsigned int BUTTON_BOX_WIDTH = 200;
    static constexpr unsigned int BUTTON_BOX_HEIGHT = 50;
    static constexpr unsigned int BUTTON_WIDTH = 120;
    static constexpr unsigned int BUTTON_HEIGHT = 32;
    static constexpr unsigned int TEXT_EDIT_WIDTH = 300;
    bool isGenerationComplete;
    bool isLastMinProcessed;
    bool isHangingSourcesProcessed;
    bool isLastQueueStepsProcessed;
    bool isSimulationFinished;
    bool queueStep;
    unsigned int currentSourceIndex;
    unsigned int prevSourceIndex;
    unsigned int currentDeviceIndex;

    void initTables();
private slots:
    void onButtonNextClicked();
};

#endif // STEPBYSTEPWINDOW_H
