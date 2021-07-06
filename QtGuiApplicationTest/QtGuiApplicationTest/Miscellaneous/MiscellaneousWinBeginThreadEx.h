#ifndef MISCELLANEOUS_WIN_BEGIN_THREAD_EX_H
#define MISCELLANEOUS_WIN_BEGIN_THREAD_EX_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousWinBeginThreadEx.h"

class MiscellaneousWinBeginThreadEx : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinBeginThreadEx(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinBeginThreadEx();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void Run();
    static unsigned int _stdcall ThreadProc(void *pArg);

private slots:
    void on_btnWinApiThreadStart_clicked();
    void on_btnWinApiThreadStop_clicked();

private:
    Ui::MiscellaneousWinBeginThreadEx ui;
    unsigned long long             m_beginThreadHandle; // _beginthreadex 返回值，线程具备，用于 WaitForSingleObject
    volatile bool                  m_threadStopFlag;
};

#endif // MISCELLANEOUS_WIN_BEGIN_THREAD_EX_H
