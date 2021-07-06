#pragma once

#include <DebugInfoBaseWidget.h>

namespace Ui
{
    class DebugInfoTestWidget;
}

class DebugInfoTestWidget : public DebugInfoBaseWidget
{
    Q_OBJECT
public:
    explicit DebugInfoTestWidget(QWidget *parent = 0);
    ~DebugInfoTestWidget();
    void UpdateLabelTest(int endNum);

protected:
    virtual bool OnDebugMenuEvent(DebugMenuEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;

private:
    Ui::DebugInfoTestWidget       *ui;
    QTimer                        *mpUpdateTimer;
};
