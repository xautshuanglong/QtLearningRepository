#include "DicomDownloadWigdet.h"
#include "ui_DicomDownloadWigdet.h"

DicomDownloadWigdet::DicomDownloadWigdet(QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::DicomDownloadWigdet();
    ui->setupUi(this);
}

DicomDownloadWigdet::~DicomDownloadWigdet()
{
    delete ui;
}
