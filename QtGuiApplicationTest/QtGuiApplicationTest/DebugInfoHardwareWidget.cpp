#include "DebugInfoHardwareWidget.h"
#include "ui_DebugInfoHardwareWidget.h"

#include <QMetaEnum>
#include <QTimer>

#include <LogUtil.h>
#include "DebugMenuEvent.h"
#include "DebugPanel.h"
#include "Win32PerformanceUtil.h"

DebugInfoHardwareWidget::DebugInfoHardwareWidget(QWidget *parent /* = 0 */)
    : DebugInfoBaseWidget(parent)
    , ui(new Ui::DebugInfoHardwareWidget)
{
    ui->setupUi(this);
    mpUpdateTimer = new QTimer(this);
    this->connect(mpUpdateTimer, SIGNAL(timeout()), SLOT(SlotUpdateCpuUsage()));
}

DebugInfoHardwareWidget::~DebugInfoHardwareWidget()
{
}

bool DebugInfoHardwareWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    int menuID = event->GetMenuID();
    QMetaEnum menuIDEnum = QMetaEnum::fromType<DebugPanel::MenuID>();
    LogUtil::Info(CODE_LOCATION, "Do not specify MenuID = %s", menuIDEnum.valueToKey(menuID));

    return true;
}

void DebugInfoHardwareWidget::showEvent(QShowEvent *event)
{
    mpUpdateTimer->start(1000);
    //LogUtil::Debug(CODE_LOCATION, "Show this widget ...");
}

void DebugInfoHardwareWidget::hideEvent(QHideEvent *event)
{
    mpUpdateTimer->stop();
    //LogUtil::Debug(CODE_LOCATION, "Hide this widget ...");
}

void DebugInfoHardwareWidget::SlotUpdateCpuUsage()
{
    double cpuUsageTotal = Win32PerformanceUtil::GetCpuUsageSystem();
    double cpuUsageProcess = Win32PerformanceUtil::GetCpuUsageProcess();

    ui->lbCpuUsageTotal->setText(QString("CPU Usage Total : %1 %").arg(cpuUsageTotal, 0, 'f', 2));
    ui->lbCpuUsageProcess->setText(QString("CPU Usage Process : %1 %").arg(cpuUsageProcess, 0, 'f', 2));

    //LogUtil::Debug(CODE_LOCATION, "CpuUsageTotal: %.2lf   CpuUsageProcess: %.2lf", cpuUsageTotal, cpuUsageProcess);
}

void DebugInfoHardwareWidget::on_btnRefreshCpuUsage_clicked()
{
    this->SlotUpdateCpuUsage();
}
