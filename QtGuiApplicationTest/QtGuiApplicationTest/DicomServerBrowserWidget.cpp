#include "DicomServerBrowserWidget.h"
#include "ui_DicomServerBrowserWidget.h"

// Self Headers
#include <LogUtil.h>
#include <DicomTest.h>
#include "TitleBarWidget.h"

DicomServerBrowserWidget::DicomServerBrowserWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DicomServerBrowserWidget)
{
    ui->setupUi(this);

    QIcon winIcon(":/AppImages/Resources/images/dicom_xray.ico");
    this->setWindowIcon(winIcon);
    mpTitleBarWidget = new TitleBarWidget(this);
}

DicomServerBrowserWidget::~DicomServerBrowserWidget()
{
    delete ui;
}

void DicomServerBrowserWidget::on_btnEchoTest_clicked()
{
    DicomTest dcmTest;
    dcmTest.DcmtkTestEcho();
}

void DicomServerBrowserWidget::on_btnFindTest_clicked()
{
    DicomTest dcmTest;
    dcmTest.DcmtkTestFind();
}

void DicomServerBrowserWidget::on_btnGetTest_clicked()
{
    DicomTest dcmTest;
    dcmTest.DcmtkTestGet();
}

void DicomServerBrowserWidget::on_btnMoveTest_clicked()
{
}

void DicomServerBrowserWidget::on_btnStoreTest_clicked()
{
    DicomTest dcmTest;
    dcmTest.DcmtkTestStore();
}
