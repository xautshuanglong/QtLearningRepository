#include "DebugInfoTestWidget.h"
#include "ui_DebugInfoTestWidget.h"

#include <QMetaEnum>
#include <QTimer>

#include "JCB_Logger/LogUtil.h"
#include "Debugger/DebugMenuEvent.h"
#include "Debugger/DebugPanel.h"
#include "Utils/Win32PerformanceUtil.h"

DebugInfoTestWidget::DebugInfoTestWidget(QWidget *parent /* = 0 */)
    : DebugInfoBaseWidget(parent)
    , ui(new Ui::DebugInfoTestWidget)
{
    ui->setupUi(this);
}

DebugInfoTestWidget::~DebugInfoTestWidget()
{
    delete ui;
}

void DebugInfoTestWidget::UpdateLabelTest(int endNum)
{
    ui->lbTest->setText(QString("DebugInfoTestWidget ") + QString::number(endNum));
}

bool DebugInfoTestWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    int menuID = event->GetMenuID();
    QMetaEnum menuIDEnum = QMetaEnum::fromType<DebugPanel::MenuID>();
    LogUtil::Info(CODE_LOCATION, "Do not specify MenuID = %s handler ...", menuIDEnum.valueToKey(menuID));

    return true;
}

void DebugInfoTestWidget::showEvent(QShowEvent *event)
{
    //LogUtil::Debug(CODE_LOCATION, "Show this widget ...");
}

void DebugInfoTestWidget::hideEvent(QHideEvent *event)
{
    //LogUtil::Debug(CODE_LOCATION, "Hide this widget ...");
}