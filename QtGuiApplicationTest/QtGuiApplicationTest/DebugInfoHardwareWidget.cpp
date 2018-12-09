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

    // 当前网络连接状态
    QStringList header;
    header << "Protocol" << "LocalAddress" << "RemoteAddress" << "Status";
    ui->twNetConnection->setColumnCount(4);
    ui->twNetConnection->setHorizontalHeaderLabels(header);
    ui->twNetConnection->verticalHeader()->setVisible(false);
    ui->twNetConnection->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twNetConnection->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twNetConnection->horizontalHeader()->setSectionsClickable(false);
    ui->twNetConnection->setFocusPolicy(Qt::NoFocus);
}

DebugInfoHardwareWidget::~DebugInfoHardwareWidget()
{
}

bool DebugInfoHardwareWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    int menuID = event->GetMenuID();
    QMetaEnum menuIDEnum = QMetaEnum::fromType<DebugPanel::MenuID>();
    LogUtil::Info(CODE_LOCATION, "Do not specify MenuID = %s handler ...", menuIDEnum.valueToKey(menuID));

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

    QList<Win32PerformanceUtil::TcpConnections> listConnectionTcp;
    Win32PerformanceUtil::GetExtendTcpTableInfo(AF_INET, listConnectionTcp);
    QList<Win32PerformanceUtil::UdpConnections> listConnectionUdp;
    Win32PerformanceUtil::GetExtendUdpTableInfo(AF_INET, listConnectionUdp);
    int listCountTcp = listConnectionTcp.count();
    int listCountUdp = listConnectionUdp.count();
    int totalCount = listCountTcp + listCountUdp;
    int rowCount = ui->twNetConnection->rowCount();
    for (int i = 0; i < listCountTcp; ++i)
    {
        if (i >= rowCount)
        {
            ui->twNetConnection->insertRow(i);
        }
        ui->twNetConnection->setItem(i, 0, new QTableWidgetItem("TCP"));
        ui->twNetConnection->setItem(i, 1, new QTableWidgetItem(QString("%1%2%3").arg(listConnectionTcp.at(i).localAddress).arg(":").arg(listConnectionTcp.at(i).localPort)));
        ui->twNetConnection->setItem(i, 2, new QTableWidgetItem(QString("%1%2%3").arg(listConnectionTcp.at(i).remoteAddress).arg(":").arg(listConnectionTcp.at(i).remotePort)));
        ui->twNetConnection->setItem(i, 3, new QTableWidgetItem(listConnectionTcp.at(i).status));
    }
    for (int i = 0; i < listCountUdp; ++i)
    {
        if (i+listCountTcp >= rowCount)
        {
            ui->twNetConnection->insertRow(i);
        }
        ui->twNetConnection->setItem(i+listCountTcp, 0, new QTableWidgetItem("UDP"));
        ui->twNetConnection->setItem(i+listCountTcp, 1, new QTableWidgetItem(QString("%1%2%3").arg(listConnectionUdp.at(i).localAddress).arg(":").arg(listConnectionUdp.at(i).localPort)));
        ui->twNetConnection->setItem(i+listCountTcp, 2, new QTableWidgetItem("*:*"));
        ui->twNetConnection->setItem(i+listCountTcp, 3, new QTableWidgetItem("*"));
    }
    for (int i = rowCount; i >= totalCount; --i)
    {
        ui->twNetConnection->removeRow(i);
    }
}

void DebugInfoHardwareWidget::on_btnRefresh_clicked()
{
    this->SlotUpdateHardwareInfo();
}
