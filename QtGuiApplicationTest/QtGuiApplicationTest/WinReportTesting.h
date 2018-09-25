#pragma once

#include <QMainWindow>

class FramelessWindowHelper;

namespace Ui
{
    class WinReportTesting;
}

class WinReportTesting : public QMainWindow
{
private:
    Ui::WinReportTesting   *ui;
    FramelessWindowHelper  *mpFramelessHelper;

public:
    explicit WinReportTesting(QWidget *parent = Q_NULLPTR);
    ~WinReportTesting();
};
