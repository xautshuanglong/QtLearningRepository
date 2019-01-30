#pragma once

#include "DebugInfoBaseWidget.h"

namespace Ui
{
    class DebugInfoNetworkWidget;
};

class DebugInfoNetworkWidget : public DebugInfoBaseWidget
{
    Q_OBJECT

public:
    DebugInfoNetworkWidget(QWidget *parent = Q_NULLPTR);
    ~DebugInfoNetworkWidget();

protected:
    virtual bool OnDebugMenuEvent(DebugMenuEvent *event) override;
    virtual void OnUpdateDebugInfo() override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_btnPingTest_clicked();

private:
    Ui::DebugInfoNetworkWidget *ui;
};
