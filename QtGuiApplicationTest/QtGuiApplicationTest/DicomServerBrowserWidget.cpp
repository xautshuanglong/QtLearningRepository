#include "DicomServerBrowserWidget.h"
#include "ui_DicomServerBrowserWidget.h"

// Self Headers
#include <LogUtil.h>
#include <DicomTest.h>
#include <DicomClient.h>

#include "TitleBarWidget.h"

DicomServerBrowserWidget::DicomServerBrowserWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DicomServerBrowserWidget)
    , mpDicomClient(new DicomClient(this))
{
    ui->setupUi(this);

    QIcon winIcon(":/AppImages/Resources/images/dicom_xray.ico");
    this->setWindowIcon(winIcon);
    mpTitleBarWidget = new TitleBarWidget(this);

    mpDicomClient->SetAppAETitle("MGIUSCLIENT");
    mpDicomClient->SetPeerAETitle("MGIUSDICOM");
    mpDicomClient->SetPeerHostName("10.225.12.64");
    mpDicomClient->SetPeerPort(7893);
    mpDicomClient->SetMaxReceivePDULength(32768);
}

DicomServerBrowserWidget::~DicomServerBrowserWidget()
{
    delete ui;
}

void DicomServerBrowserWidget::on_btnEchoTest_clicked()
{
    mpDicomClient->PerformEcho();

    //DicomTest dcmTest;
    //dcmTest.DcmtkTestEcho();

    //DicomEchoSCU echoSCU;
    //echoSCU.PerformEcho();
}

void DicomServerBrowserWidget::on_btnFindTest_clicked()
{
    mpDicomClient->PerformFind();

    //DicomTest dcmTest;
    //dcmTest.DcmtkTestFind();

    //DicomFindSCU findSCU;
    //findSCU.InitializeNetwork();
    //findSCU.SetBlockingMode(false); // 非阻塞模式 select
    //findSCU.SetAppEntityTitle("DebugTest", "DICOM");
    //findSCU.AppendOverrideKey("QueryRetrieveLevel=STUDY");
    //findSCU.AppendOverrideKey("PatientName");
    //findSCU.AppendOverrideKey("PatientID");
    //findSCU.AppendOverrideKey("NumberOfStudyRelatedInstances");
    //findSCU.PerformQuery();
    //findSCU.DropNetwork();
}

void DicomServerBrowserWidget::on_btnGetTest_clicked()
{
    mpDicomClient->PerformGet();

    //DicomTest dcmTest;
    //dcmTest.DcmtkTestGet();
}

void DicomServerBrowserWidget::on_btnMoveTest_clicked()
{
    mpDicomClient->PerformMove();
}

void DicomServerBrowserWidget::on_btnStoreTest_clicked()
{
    QString dcmFilename = ui->leDcmFilename->text();
    if (!dcmFilename.isEmpty())
    {
        mpDicomClient->MakeStoreTask(dcmFilename);
    }
    else
    {
        // TODO: UI 输入错误
    }
}
