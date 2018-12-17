#pragma once

#include <DebugInfoBaseWidget.h>

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

protected:
    virtual bool OnDebugMenuEvent(DebugMenuEvent *event) override;
    virtual void OnUpdateDebugInfo() override;

private slots:
    void on_btnRefresh_clicked();

private:
    Ui::DebugInfoHardwareWidget    *ui;
    QTimer                         *mpUpdateTimer;
};
