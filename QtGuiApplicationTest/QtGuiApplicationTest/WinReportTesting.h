#pragma once

#include <QMainWindow>
#include "MuPDF.h"

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

public:
    explicit WinReportTesting(QWidget *parent = Q_NULLPTR);
    ~WinReportTesting();

private:
    void QTextDocumentTest();
    void QTextEditDocumentTest();

private slots:
    void on_btnPrintTest_clicked();
    void on_btnFormatTest_clicked();
    void on_btnSavePDF_clicked();
    void on_btnPreviewFOP_clicked();
    void on_btnPreviewMuPDF_clicked();
    void on_btnPrintImgPDF_clicked();
    void on_btnMuPdfWrap_clicked();
    void SlotMuPdfError(MuPDF::ErrorCode code, QString errorString);
};
