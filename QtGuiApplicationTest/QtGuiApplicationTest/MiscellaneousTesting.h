#pragma once

#include <QMainWindow>
#include <QMap>
#include "MiscellaneousDefines.h"

class FramelessWindowHelper;
class QTreeWidgetItem;
class MiscellaneousBase;

namespace Ui
{
    class MiscellaneousTesting;
}

class MiscellaneousTesting : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MiscellaneousTesting                        *ui;
    FramelessWindowHelper                           *mpFramelessHelper;
    QMap<MiscellaneousTestGroup, QTreeWidgetItem*>   mMapTestGroup;
    QMap<MiscellaneousTestItem, int>                 mMapTestPageIndex;
    // WinAPI �̲߳���
    unsigned long long          m_beginThreadHandle; // _beginthreadex ����ֵ���߳̾߱������� WaitForSingleObject
    bool                        m_threadStopFlag;

public:
    explicit MiscellaneousTesting(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousTesting();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void InitializeUI();
    void AppendTestPage(MiscellaneousBase* pWidgetPage);
    void Run();
    static unsigned int _stdcall ThreadProc(void *pArg);

signals:
    void SignalClosed();

private slots:
    void on_btnWinApiThreadStart_clicked();
    void on_btnWinApiThreadStop_clicked();
    void on_btnQPrinterTest_clicked();
    void on_twMiscellaneousTesting_itemClicked(QTreeWidgetItem *item, int column);
};
