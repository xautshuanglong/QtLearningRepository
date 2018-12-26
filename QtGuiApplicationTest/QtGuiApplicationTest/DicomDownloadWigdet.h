#pragma once

#include <QWidget>
namespace Ui
{
    class DicomDownloadWigdet;
};

class DicomDownloadWigdet : public QWidget
{
    Q_OBJECT

public:
    DicomDownloadWigdet(QWidget *parent = Q_NULLPTR);
    ~DicomDownloadWigdet();

private:
    Ui::DicomDownloadWigdet *ui;
};
