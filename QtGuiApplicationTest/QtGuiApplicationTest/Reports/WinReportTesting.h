#pragma once

#include <QMainWindow>
#include "MuPDF.h"
#include "FopReportClient.h"

class FramelessWindowHelper;
class QTcpSocket;
class QLabel;

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
    QMap<int, QLabel*>      mMapPageNumImage;

public:
    explicit WinReportTesting(QWidget *parent = Q_NULLPTR);
    ~WinReportTesting();

private:
    void ShowPdfImage(QImage pdfPage);

private slots:
    void on_btnSavePDF_clicked();
    void on_btnPreviewFOP_clicked();
    void on_btnPreviewMuPDF_clicked();
    void on_btnPrintImgPDF_clicked();
    void on_btnMuPdfWrap_clicked();
    void on_btnMuPdfWrapPre_clicked();
    void on_btnMuPdfWrapNext_clicked();
    void on_btnMuPdfScale_clicked();
    void on_btnMuPdfRotate_clicked();
    void on_btnMuPdfTranslate_clicked();
    void SlotMuPdfError(MuPDF::ErrorCode code, QString errorString);
    void SlotSaveCompletely();
};
