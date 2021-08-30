#ifndef MISCELLANEOUS_WIN_THREAD_H
#define MISCELLANEOUS_WIN_THREAD_H

#include <mutex>
#include <shared_mutex>

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousWinThread.h"

class MiscellaneousWinThread : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinThread(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinThread();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void TestHelperMutex();
    void TestHelperRecursiveMutex();
    void Run();
    static unsigned int _stdcall ThreadProc(void *pArg);

private slots:
    void on_btnWinApiThreadStart_clicked();
    void on_btnWinApiThreadStop_clicked();
    void on_btnEmptyTest1_clicked();
    void on_btnEmptyTest2_clicked();

private:
    Ui::MiscellaneousWinThread ui;
    unsigned long long             m_beginThreadHandle; // _beginthreadex 返回值，线程具备，用于 WaitForSingleObject
    volatile bool                  m_threadStopFlag;
    std::mutex                     m_mutexTest;
    std::recursive_mutex           m_recursiveMutexTest;
};

#endif // MISCELLANEOUS_WIN_THREAD_H
