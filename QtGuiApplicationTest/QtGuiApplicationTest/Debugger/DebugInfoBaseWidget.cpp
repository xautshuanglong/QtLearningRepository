#include "DebugInfoBaseWidget.h"
#include "ui_DebugPanel.h"

#include <QLabel>

#include <LogUtil.h>
#include "DebugMenuEvent.h"

DebugInfoBaseWidget::DebugInfoBaseWidget(QWidget *parent)
    : QWidget(parent)
{
}

DebugInfoBaseWidget::~DebugInfoBaseWidget()
{
}

bool DebugInfoBaseWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    LogUtil::Warn(CODE_LOCATION, "Do not specify DebugInfoWidget MenuID = %d", event->GetMenuID());
    return true;
}

void DebugInfoBaseWidget::OnUpdateDebugInfo()
{
    LogUtil::Warn(CODE_LOCATION, "nothing to update");
}
