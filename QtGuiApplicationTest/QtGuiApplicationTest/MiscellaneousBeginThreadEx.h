#ifndef MISCELLANEOUS_WINAPI_MULTI_THREAD_H
#define MISCELLANEOUS_WINAPI_MULTI_THREAD_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousBeginThreadEx.h"

class MiscellaneousBeginThreadEx : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousBeginThreadEx(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousBeginThreadEx();

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
    Ui::MiscellaneousBeginThreadEx ui;
    unsigned long long             m_beginThreadHandle; // _beginthreadex 返回值，线程具备，用于 WaitForSingleObject
    volatile bool                  m_threadStopFlag;
};

#endif // MISCELLANEOUS_WINAPI_MULTI_THREAD_H
