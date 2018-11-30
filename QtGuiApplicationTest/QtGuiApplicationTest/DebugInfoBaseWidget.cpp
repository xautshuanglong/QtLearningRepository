#include "DebugInfoBaseWidget.h"
#include "ui_DebugPanel.h"

#include <QLabel>

#include <LogUtil.h>

DebugInfoBaseWidget::DebugInfoBaseWidget(QWidget *parent)
    : QWidget(parent)
{
    QLabel *pTempLabel = new QLabel("Test Item", this);
}

DebugInfoBaseWidget::~DebugInfoBaseWidget()
{
}

bool DebugInfoBaseWidget::DebugMenuEvent(DebugEvent *event)
{
    LogUtil::Warn(CODE_LOCATION, "Do not specify DebugInfoWidget ...");
    return true;
}
