#pragma once

#include <QMainWindow>
#include "MuPDF.h"
#include "FopReportClient.h"

class FramelessWindowHelper;
class QTcpSocket;

namespace Ui
{
    class WinReportTesting;
}

class WinReportTesting : public QMainWindow
{
    Q_OBJECT
private:
    Ui::WinReportTesting   *ui;
    FramelessWindowHelper  *mpFramelessHelper;
    QTcpSocket             *mpSocketClient1;
    QTcpSocket             *mpSocketClient2;
    MuPDF                   mPdfPreview;
    FopReportClient         mFopClient;

public:
    explicit WinReportTesting(QWidget *parent = Q_NULLPTR);
    ~WinReportTesting();

private:

private slots:
    void on_btnSavePDF_clicked();
    void on_btnPreviewFOP_clicked();
    void on_btnPreviewMuPDF_clicked();
    void on_btnPrintImgPDF_clicked();
    void on_btnMuPdfWrap_clicked();
    void on_btnMuPdfWrapPre_clicked();
    void on_btnMuPdfWrapNext_clicked();
    void SlotMuPdfError(MuPDF::ErrorCode code, QString errorString);
    void SlotSaveCompletely();
};
