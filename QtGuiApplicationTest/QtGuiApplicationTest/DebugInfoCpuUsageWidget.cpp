#include "DebugInfoCpuUsageWidget.h"
#include "ui_DebugInfoCpuUsageWidget.h"

#include <QMetaEnum>

#include <LogUtil.h>
#include "DebugMenuEvent.h"
#include "DebugPanel.h"
#include "Win32PerformanceUtil.h"

DebugInfoCpuUsageWidget::DebugInfoCpuUsageWidget(QWidget *parent /* = 0 */)
    : DebugInfoBaseWidget(parent)
    , ui(new Ui::DebugInfoCpuUsageWidget)
{
    ui->setupUi(this);
}

DebugInfoCpuUsageWidget::~DebugInfoCpuUsageWidget()
{
}

bool DebugInfoCpuUsageWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    int menuID = event->GetMenuID();
    QMetaEnum menuIDEnum = QMetaEnum::fromType<DebugPanel::MenuID>();
    LogUtil::Debug(CODE_LOCATION, "Do not specify MenuID = %s", menuIDEnum.valueToKey(menuID));

    return true;
}

void DebugInfoCpuUsageWidget::on_btnRefreshCpuUsage_clicked()
{
    double cpuUsageTotal = Win32PerformanceUtil::GetCpuUsageSystem();
    double cpuUsageProcess = Win32PerformanceUtil::GetCpuUsageProcess();

    ui->lbCpuUsageTotal->setText(QString("CPU Usage Total : %1 %").arg(cpuUsageTotal));
    ui->lbCpuUsageProcess->setText(QString("CPU Usage Process : %1 %").arg(cpuUsageProcess));
}
