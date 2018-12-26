#include "DicomServerBrowserWidget.h"
#include "ui_DicomServerBrowserWidget.h"
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
