#pragma once

#include <QMainWindow>

class FramelessWindowHelper;

namespace Ui
{
    class MiscellaneousTesting;
}

class MiscellaneousTesting : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MiscellaneousTesting   *ui;
    FramelessWindowHelper      *mpFramelessHelper;
    // WinAPI 线程测试
    unsigned long long          m_beginThreadHandle; // _beginthreadex 返回值，线程具备，用于 WaitForSingleObject
    bool                        m_threadStopFlag;

public:
    explicit MiscellaneousTesting(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousTesting();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    static unsigned int _stdcall ThreadProc(void *pArg);
    void run();

signals:
    void SignalClosed();

private slots:
    void on_btnWinApiThreadStart_clicked();
    void on_btnWinApiThreadStop_clicked();
    void on_btnQPrinterTest_clicked();
};
