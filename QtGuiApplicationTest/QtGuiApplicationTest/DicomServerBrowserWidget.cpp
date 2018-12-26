#include "DicomServerBrowserWidget.h"
#include "ui_DicomServerBrowserWidget.h"

DicomServerBrowserWidget::DicomServerBrowserWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DicomServerBrowserWidget)
{
    ui->setupUi(this);
}

DicomServerBrowserWidget::~DicomServerBrowserWidget()
{
    delete ui;
}
