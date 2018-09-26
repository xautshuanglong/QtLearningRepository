#pragma once

#include <QMainWindow>

class FramelessWindowHelper;

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

public:
    explicit WinReportTesting(QWidget *parent = Q_NULLPTR);
    ~WinReportTesting();

private:
    void QTextDocumentTest();

private slots:
    void on_btnPrintTest_clicked();
};
