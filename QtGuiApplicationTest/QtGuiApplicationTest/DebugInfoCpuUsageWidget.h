#pragma once

#include <DebugInfoBaseWidget.h>

namespace Ui
{
    class DebugInfoCpuUsageWidget;
}

class DebugInfoCpuUsageWidget : public DebugInfoBaseWidget
{
    Q_OBJECT
public:
    explicit DebugInfoCpuUsageWidget(QWidget *parent = 0);
    ~DebugInfoCpuUsageWidget();

protected:
    virtual bool OnDebugMenuEvent(DebugMenuEvent *event) override;

private slots:
    void on_btnRefreshCpuUsage_clicked();

private:
    Ui::DebugInfoCpuUsageWidget    *ui;
};
