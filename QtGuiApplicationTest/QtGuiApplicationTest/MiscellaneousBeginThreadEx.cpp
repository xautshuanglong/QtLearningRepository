#include "MiscellaneousBeginThreadEx.h"

#include <windows.h>
#include <process.h>

MiscellaneousBeginThreadEx::MiscellaneousBeginThreadEx(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
    , m_threadStopFlag(true)
{
    ui.setupUi(this);
}

MiscellaneousBeginThreadEx::~MiscellaneousBeginThreadEx()
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

QString MiscellaneousBeginThreadEx::GetTitle()
{
    return QObject::tr("BeginThreadEx");
}

QString MiscellaneousBeginThreadEx::GetTitleTooltip()
{
    return QObject::tr("How to use windows API create sub thread.");
}

MiscellaneousTestGroup MiscellaneousBeginThreadEx::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousBeginThreadEx::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Thread_beginthreadex;
}

void MiscellaneousBeginThreadEx::on_btnWinApiThreadStart_clicked()
{
    if (m_threadStopFlag)
    {
        m_threadStopFlag = false;
        m_beginThreadHandle = _beginthreadex(NULL, 0, MiscellaneousBeginThreadEx::ThreadProc, (void*)this, 0, NULL);
    }
}

void MiscellaneousBeginThreadEx::on_btnWinApiThreadStop_clicked()
{
    m_threadStopFlag = true;
    ::WaitForSingleObject((HANDLE)m_beginThreadHandle, INFINITE);
    int i = 0;
}

void MiscellaneousBeginThreadEx::Run()
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

unsigned int _stdcall MiscellaneousBeginThreadEx::ThreadProc(void *pArg)
{
    MiscellaneousBeginThreadEx *pMiscellaneous = (MiscellaneousBeginThreadEx*)pArg;
    pMiscellaneous->Run();
    return 0;
}
