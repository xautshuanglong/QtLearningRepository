#include "MiscellaneousWinBeginThreadEx.h"

#include <windows.h>
#include <process.h>

MiscellaneousWinBeginThreadEx::MiscellaneousWinBeginThreadEx(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
    , m_threadStopFlag(true)
{
    ui.setupUi(this);
}

MiscellaneousWinBeginThreadEx::~MiscellaneousWinBeginThreadEx()
{
    if (m_threadStopFlag)
    {
        int i = 0;
    }
    else
    {
        int i = 0;
    }
}

QString MiscellaneousWinBeginThreadEx::GetTitle()
{
    return QObject::tr("BeginThreadEx");
}

QString MiscellaneousWinBeginThreadEx::GetTitleTooltip()
{
    return QObject::tr("How to use windows API create sub thread.");
}

MiscellaneousTestGroup MiscellaneousWinBeginThreadEx::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinBeginThreadEx::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Thread_BeginThreadEx;
}

void MiscellaneousWinBeginThreadEx::on_btnWinApiThreadStart_clicked()
{
    if (m_threadStopFlag)
    {
        m_threadStopFlag = false;
        m_beginThreadHandle = _beginthreadex(NULL, 0, MiscellaneousWinBeginThreadEx::ThreadProc, (void*)this, 0, NULL);
    }
}

void MiscellaneousWinBeginThreadEx::on_btnWinApiThreadStop_clicked()
{
    m_threadStopFlag = true;
    ::WaitForSingleObject((HANDLE)m_beginThreadHandle, INFINITE);
    int i = 0;
}

void MiscellaneousWinBeginThreadEx::Run()
{
    int count = 0;
    while (count < 1000)
    {
        ::_sleep(10);
        ++count;
        if (m_threadStopFlag)
        {
            break;
        }
    }
    m_threadStopFlag = true;
}

unsigned int _stdcall MiscellaneousWinBeginThreadEx::ThreadProc(void *pArg)
{
    MiscellaneousWinBeginThreadEx *pMiscellaneous = (MiscellaneousWinBeginThreadEx*)pArg;
    pMiscellaneous->Run();
    return 0;
}
