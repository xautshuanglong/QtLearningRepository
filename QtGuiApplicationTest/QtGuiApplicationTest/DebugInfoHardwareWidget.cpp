#include "DebugInfoHardwareWidget.h"
#include "ui_DebugInfoHardwareWidget.h"

// QT Headers
#include <QMetaEnum>
#include <QTimer>
#include <QLineSeries>
#include <QChar>
#include <QChartView>
#include <QValueAxis>

#include <LogUtil.h>
#include "DebugMenuEvent.h"
#include "DebugPanel.h"
#include "Win32PerformanceUtil.h"
#include "DebugInfoTestWidget.h"

QT_CHARTS_USE_NAMESPACE

DebugInfoHardwareWidget::DebugInfoHardwareWidget(QWidget *parent /* = 0 */)
    : DebugInfoBaseWidget(parent)
    , ui(new Ui::DebugInfoHardwareWidget)
    , mAxisRangeX(50)
    , mAxisRangeY(100)
{
    ui->setupUi(this);

    this->InitChartView();
    this->InitNetConnectionList(); // 当前网络连接状态
}

DebugInfoHardwareWidget::~DebugInfoHardwareWidget()
{
    delete ui;
}

void DebugInfoHardwareWidget::InitChartView()
{
    mpLineSeriesCpuTotal = new QLineSeries(this);
    mpLineSeriesCpuTotal->setName(tr("Total"));
    mpLineSeriesCpuProcess = new QLineSeries(this);
    mpLineSeriesCpuProcess->setName(tr("Process"));

    for (int i=0; i<mAxisRangeX; ++i)
    {
        mpLineSeriesCpuTotal->append(i, 0);
        mpLineSeriesCpuProcess->append(i, 0);
    }

    QChart *chart = new QChart();
    chart->addSeries(mpLineSeriesCpuTotal);
    chart->addSeries(mpLineSeriesCpuProcess);
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, mAxisRangeX);
    chart->axisX()->setLabelsVisible(false);
    chart->axisY()->setRange(0, mAxisRangeY);
    chart->setTitle("CPU Usage Rate");
    chart->legend()->setAlignment(Qt::AlignBottom);
    //chart->setTheme(QT_CHARTS_NAMESPACE::QChart::ChartTheme::ChartThemeBrownSand);
    //chart->setBackgroundVisible(false);

    mpChartView = new QChartView(chart, ui->topLeftWidget);
    mpChartView->setRenderHint(QPainter::Antialiasing);
    QPalette chartPalette = mpChartView->palette();
    chartPalette.setColor(QPalette::Background, QColor(255, 255, 0, 0));
    mpChartView->setPalette(chartPalette);
}

void DebugInfoHardwareWidget::InitNetConnectionList()
{
    QStringList header;
    header << "Protocol" << "LocalAddress" << "RemoteAddress" << "Status";
    ui->twNetConnection->setColumnCount(4);
    ui->twNetConnection->setHorizontalHeaderLabels(header);
    ui->twNetConnection->verticalHeader()->setVisible(false);
    ui->twNetConnection->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twNetConnection->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twNetConnection->horizontalHeader()->setSectionsClickable(false);
    ui->twNetConnection->setFocusPolicy(Qt::NoFocus);

    ui->twNetConnection->setColumnWidth(0, 80);  // 协议族
    ui->twNetConnection->setColumnWidth(1, 120); // 本地 IP:Port
    ui->twNetConnection->setColumnWidth(2, 120); // 远程 IP:Port
    ui->twNetConnection->setColumnWidth(3, 150); // 网络状态
}

void DebugInfoHardwareWidget::UpdateCpuUsageRate()
{
    double cpuUsageTotal = Win32PerformanceUtil::GetCpuUsageSystem();
    double cpuUsageProcess = Win32PerformanceUtil::GetCpuUsageProcess();
    ui->lbCpuUsageTotal->setText(QString("CPU Usage Total : %1 %").arg(cpuUsageTotal, 0, 'f', 2));
    ui->lbCpuUsageProcess->setText(QString("CPU Usage Process : %1 %").arg(cpuUsageProcess, 0, 'f', 2));
    //LogUtil::Debug(CODE_LOCATION, "CpuUsageTotal: %.2lf   CpuUsageProcess: %.2lf", cpuUsageTotal, cpuUsageProcess);

    QVector<QPointF> pointsCpuTotal = mpLineSeriesCpuTotal->pointsVector();
    QVector<QPointF> pointsCpuProcess = mpLineSeriesCpuProcess->pointsVector();
    int pointCount = pointsCpuTotal.count();
    for (int i=0; i<pointCount-1; ++i)
    {
        pointsCpuTotal[i].setY(pointsCpuTotal[i+1].y());
        pointsCpuProcess[i].setY(pointsCpuProcess[i+1].y());
    }
    pointsCpuTotal[pointCount-1].setY(cpuUsageTotal);
    pointsCpuProcess[pointCount - 1].setY(cpuUsageProcess);
    mpLineSeriesCpuTotal->replace(pointsCpuTotal);
    mpLineSeriesCpuProcess->replace(pointsCpuProcess);
}

void DebugInfoHardwareWidget::UpdateMemoryUsageRate()
{
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
    //LogUtil::Debug(CODE_LOCATION, "Percent: %u  MemoryTotal: %uMB   MemoryUsed: %uMB  ProcessWorkingSet: %ullKB  ProcessPagefileUsage: %ullKB",
    //               memLoadPercent, memTotalMB, memUsedMB, processWorkingSetKB, processPagefileUsageKB);
}

void DebugInfoHardwareWidget::UpdateNetConnections()
{
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
        if (i + listCountTcp >= rowCount)
        {
            ui->twNetConnection->insertRow(i);
        }
        ui->twNetConnection->setItem(i + listCountTcp, 0, new QTableWidgetItem("UDP"));
        ui->twNetConnection->setItem(i + listCountTcp, 1, new QTableWidgetItem(QString("%1%2%3").arg(listConnectionUdp.at(i).localAddress).arg(":").arg(listConnectionUdp.at(i).localPort)));
        ui->twNetConnection->setItem(i + listCountTcp, 2, new QTableWidgetItem("*:*"));
        ui->twNetConnection->setItem(i + listCountTcp, 3, new QTableWidgetItem("*"));
    }
    for (int i = rowCount; i >= totalCount; --i)
    {
        ui->twNetConnection->removeRow(i);
    }
}

bool DebugInfoHardwareWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    int menuID = event->GetMenuID();
    QMetaEnum menuIDEnum = QMetaEnum::fromType<DebugPanel::MenuID>();
    LogUtil::Info(CODE_LOCATION, "Do not specify MenuID = %s handler ...", menuIDEnum.valueToKey(menuID));

    return true;
}

void DebugInfoHardwareWidget::OnUpdateDebugInfo()
{
    this->UpdateCpuUsageRate();     // CPU 使用率
    this->UpdateMemoryUsageRate();  // 内存使用率
    this->UpdateNetConnections();   // 枚举当前进程 Socket 连接
}

void DebugInfoHardwareWidget::resizeEvent(QResizeEvent *event)
{
    mpChartView->resize(ui->topLeftWidget->size());
}

void DebugInfoHardwareWidget::on_btnRefresh_clicked()
{
    this->OnUpdateDebugInfo();

    // 外部操作调试面板测试
    DebugPanel::GetInstance()->GetDebugInfoWidget<DebugInfoHardwareWidget>();
    DebugInfoTestWidget *pTestWidget = DebugPanel::GetInstance()->GetDebugInfoWidget<DebugInfoTestWidget>();
    if (pTestWidget)
    {
        static int test = 0;
        pTestWidget->UpdateLabelTest(++test);
    }
}
