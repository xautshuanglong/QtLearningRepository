#pragma once

#include <QChartGlobal>
#include <DebugInfoBaseWidget.h>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChartView;
QT_CHARTS_END_NAMESPACE

namespace Ui
{
    class DebugInfoHardwareWidget;
}

class DebugInfoHardwareWidget : public DebugInfoBaseWidget
{
    Q_OBJECT
public:
    explicit DebugInfoHardwareWidget(QWidget *parent = 0);
    ~DebugInfoHardwareWidget();

private:
    void InitChartView();
    void InitNetConnectionList();
    void UpdateCpuUsageRate();
    void UpdateMemoryUsageRate();
    void UpdateNetConnections();

protected:
    virtual bool OnDebugMenuEvent(DebugMenuEvent *event) override;
    virtual void OnUpdateDebugInfo() override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_btnRefresh_clicked();

private:
    Ui::DebugInfoHardwareWidget         *ui;
    QT_CHARTS_NAMESPACE::QLineSeries    *mpLineSeriesCpuTotal;
    QT_CHARTS_NAMESPACE::QLineSeries    *mpLineSeriesCpuProcess;
    QT_CHARTS_NAMESPACE::QChartView     *mpChartView;
    int                                  mAxisRangeX;
    int                                  mAxisRangeY;
};
