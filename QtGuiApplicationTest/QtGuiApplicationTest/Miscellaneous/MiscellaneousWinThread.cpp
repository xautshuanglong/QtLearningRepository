#include "MiscellaneousWinThread.h"

#include <windows.h>
#include <process.h>

MiscellaneousWinThread::MiscellaneousWinThread(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
    , m_threadStopFlag(true)
{
    ui.setupUi(this);
}

MiscellaneousWinThread::~MiscellaneousWinThread()
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

QString MiscellaneousWinThread::GetTitle()
{
    return QObject::tr("BeginThreadEx");
}

QString MiscellaneousWinThread::GetTitleTooltip()
{
    return QObject::tr("How to use windows API create sub thread.");
}

MiscellaneousTestGroup MiscellaneousWinThread::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinThread::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Thread_BeginThreadEx;
}

void MiscellaneousWinThread::on_btnWinApiThreadStart_clicked()
{
    if (m_threadStopFlag)
    {
        m_threadStopFlag = false;
        m_beginThreadHandle = _beginthreadex(NULL, 0, MiscellaneousWinThread::ThreadProc, (void*)this, 0, NULL);
    }
}

void MiscellaneousWinThread::on_btnWinApiThreadStop_clicked()
{
    m_threadStopFlag = true;
    ::WaitForSingleObject((HANDLE)m_beginThreadHandle, INFINITE);
    int i = 0;
}

void MiscellaneousWinThread::Run()
{
    int count = 0;
    while (count < 1000)
    {
        ::_sleep(10);
        ++count;
        if (m_threadStopFlag)
        {
            ::_endthreadex(0);
            //break;
        }
    }
    m_threadStopFlag = true;
}

unsigned int _stdcall MiscellaneousWinThread::ThreadProc(void *pArg)
{
    MiscellaneousWinThread *pMiscellaneous = (MiscellaneousWinThread*)pArg;
    pMiscellaneous->Run();
    return 0;
}
