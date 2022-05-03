#include "MiscellaneousWinThread.h"

#include <windows.h>
#include <process.h>

#include "JCB_Logger/LogUtil.h"

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
    return QObject::tr("Thread");
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

void MiscellaneousWinThread::on_btnEmptyTest1_clicked()
{
    m_mutexTest.lock();
    LogUtil::Debug(CODE_LOCATION, "Inside outer locker before .................");
    this->TestHelperMutex();
    LogUtil::Debug(CODE_LOCATION, "Inside outer locker after .................");
    m_mutexTest.unlock();
}

void MiscellaneousWinThread::on_btnEmptyTest2_clicked()
{
    std::lock_guard<std::recursive_mutex> lock(m_recursiveMutexTest);
    LogUtil::Debug(CODE_LOCATION, "Inside outer recursive locker before .................");

    int count = 0;
    while (count < 100000)
    {
        this->TestHelperRecursiveMutex();
        ++count;
    }

    LogUtil::Debug(CODE_LOCATION, "Inside outer recursive locker after .................");

    //m_recursiveMutexTest.lock();
    //LogUtil::Debug(CODE_LOCATION, "Inside outer recursive locker before .................");
    //this->TestHelperRecursiveMutex();
    //LogUtil::Debug(CODE_LOCATION, "Inside outer recursive locker after .................");
    //m_recursiveMutexTest.unlock();
}

void MiscellaneousWinThread::TestHelperMutex()
{
    try
    {
        m_mutexTest.lock();
        LogUtil::Debug(CODE_LOCATION, "Inside inner locker .................");
        m_mutexTest.unlock();
    }
    catch (std::system_error& e)
    {
        LogUtil::Debug(CODE_LOCATION, "Inner lock failed : %s", e.what());
    }
}

void MiscellaneousWinThread::TestHelperRecursiveMutex()
{
    std::lock_guard<std::recursive_mutex> lock(m_recursiveMutexTest);
    LogUtil::Debug(CODE_LOCATION, "Inside inner recursive locker .................");
 
    //m_recursiveMutexTest.lock();
    //LogUtil::Debug(CODE_LOCATION, "Inside inner recursive locker .................");
    //m_recursiveMutexTest.unlock();
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
