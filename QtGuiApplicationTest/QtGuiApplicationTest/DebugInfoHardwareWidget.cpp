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

    ULONGLONG memTotal = 0, memUsed = 0, processMemUsed = 0;
    Win32PerformanceUtil::GetMemoryInfoSystem(memTotal, memUsed);
    double memUsagePercent = memUsed * 100.0 / memTotal;
    float memTotalMB = memTotal / 1024.0f / 1024.0f;
    float memUsedMB = memUsed / 1024.0f / 1024.0f;
    ui->lbMemoryUsageSystem->setText(QString("Memory Usage System: %1% ( %2MB / %3MB )")
                               .arg(memUsagePercent, 0, 'f', 2)
                               .arg(memUsedMB, 0, 'f', 2)
                               .arg(memTotalMB, 0, 'f', 2));

    Win32PerformanceUtil::GetMemoryInfoProcess(processMemUsed);
    float processMemUsedKB = processMemUsed / 1024.0f;
    ui->lbMemoryUsageProcess->setText(QString("Memory Usage Process: %1KB")
                                      .arg(processMemUsedKB, 0, 'f', 2));

    LogUtil::Debug(CODE_LOCATION, "Percent: %.2f  MemoryTotal: %.2fMB   MemoryUsed: %.2fMB  ProcessMemUsed: %.2fKB",
                   memUsagePercent, memTotalMB, memUsedMB, processMemUsedKB);
}

void DebugInfoHardwareWidget::on_btnRefresh_clicked()
{
    this->SlotUpdateHardwareInfo();
}
