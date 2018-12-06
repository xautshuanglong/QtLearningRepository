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
    this->connect(mpUpdateTimer, SIGNAL(timeout()), SLOT(SlotUpdateHardwareInfo()));
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

void DebugInfoHardwareWidget::SlotUpdateHardwareInfo()
{
    double cpuUsageTotal = Win32PerformanceUtil::GetCpuUsageSystem();
    double cpuUsageProcess = Win32PerformanceUtil::GetCpuUsageProcess();
    ui->lbCpuUsageTotal->setText(QString("CPU Usage Total : %1 %").arg(cpuUsageTotal, 0, 'f', 2));
    ui->lbCpuUsageProcess->setText(QString("CPU Usage Process : %1 %").arg(cpuUsageProcess, 0, 'f', 2));
    //LogUtil::Debug(CODE_LOCATION, "CpuUsageTotal: %.2lf   CpuUsageProcess: %.2lf", cpuUsageTotal, cpuUsageProcess);

    ULONG memLoadPercent = 0;
    ULONGLONG memTotal = 0, memAvailable = 0, memUsed = 0, workingSetSize = 0, pagefileUsage = 0;
    Win32PerformanceUtil::GetMemoryInfoSystem(memLoadPercent, memTotal, memAvailable);
    memUsed = memTotal - memAvailable;
    ULONG memTotalMB = memTotal / 1024 / 1024;
    ULONG memUsedMB = memUsed / 1024 / 1024;
    ui->lbMemoryUsageSystem->setText(QString("Memory Usage System: %1% ( %2MB / %3MB )")
                               .arg(memLoadPercent)
                               .arg(memUsedMB)
                               .arg(memTotalMB));

    Win32PerformanceUtil::GetMemoryInfoProcess(workingSetSize, pagefileUsage);
    ULONGLONG processWorkingSetKB = workingSetSize / 1024;
    ULONGLONG processPagefileUsageKB = pagefileUsage / 1024;
    ui->lbMemoryUsageProcess->setText(QString("Memory Usage Process: wokingSet=%1KB pagefileUsage=%2KB")
                                      .arg(processWorkingSetKB)
                                      .arg(processPagefileUsageKB));

    LogUtil::Debug(CODE_LOCATION, "Percent: %u  MemoryTotal: %uMB   MemoryUsed: %uMB  ProcessWorkingSet: %ullKB  ProcessPagefileUsage: %ullKB",
                   memLoadPercent, memTotalMB, memUsedMB, processWorkingSetKB, processPagefileUsageKB);
}

void DebugInfoHardwareWidget::on_btnRefresh_clicked()
{
    this->SlotUpdateHardwareInfo();
}
