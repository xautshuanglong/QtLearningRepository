﻿#include "DebugInfoBaseWidget.h"
#include "ui_DebugPanel.h"

#include <QLabel>

#include <LogUtil.h>
#include "DebugMenuEvent.h"

DebugInfoBaseWidget::DebugInfoBaseWidget(QWidget *parent)
    : QWidget(parent)
{
    QLabel *pTempLabel = new QLabel("Test Item", this);
}

DebugInfoBaseWidget::~DebugInfoBaseWidget()
{
}

bool DebugInfoBaseWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    LogUtil::Warn(CODE_LOCATION, "Do not specify DebugInfoWidget MenuID = %d", event->GetMenuID());
    return true;
}
