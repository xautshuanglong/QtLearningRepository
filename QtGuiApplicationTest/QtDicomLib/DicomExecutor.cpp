#include "DicomExecutor.h"

// QT Headers
#include <QMutexLocker>

// Self Headers
#include <LogUtil.h>

DicomExecutor::DicomExecutor(QObject *parent/* =Q_NULLPTR */)
    : QObject(parent)
    , m_AtomicRuning(0)
{
    this->setAutoDelete(false);
    m_DicomThreadPool.setExpiryTimeout(-1); // ÓÀ²»¹ýÆÚ

    LogUtil::Debug(CODE_LOCATION, " Constructed ...");
}

DicomExecutor::~DicomExecutor()
{
    LogUtil::Debug(CODE_LOCATION, " Destructed ...");
    this->Stop();
}

void DicomExecutor::AddTask(QSharedPointer<DicomTaskBase> pTask)
{
    QMutexLocker locker(&m_MutexTaskList);
    m_TaskList.push_back(pTask);
    this->WakeOne();
    LogUtil::Debug(CODE_LOCATION, "Add a task -- size: %d", m_TaskList.size());
}

void DicomExecutor::GetTask(QSharedPointer<DicomTaskBase>& pTask)
{
    LogUtil::Debug(CODE_LOCATION, "Active thread count: %d", m_DicomThreadPool.activeThreadCount());

    QMutexLocker locker(&m_MutexTaskList);
    if (!m_TaskList.empty())
    {
        pTask = m_TaskList.front();
        m_TaskList.pop_front();
        LogUtil::Debug(CODE_LOCATION, "Get a task -- size: %d", m_TaskList.size());
    }
}

void DicomExecutor::Start()
{
    if (m_AtomicRuning.testAndSetOrdered(0, 1))
    {
        m_DicomThreadPool.start(this, QThread::LowPriority);
        m_DicomThreadPool.start(this, QThread::LowPriority);
    }
}

void DicomExecutor::Stop()
{
    if (m_AtomicRuning.testAndSetOrdered(1, 0))
    {
        this->WakeAll();
        LogUtil::Debug(CODE_LOCATION, "Wait for all threads done -- ActiveCount: %d", m_DicomThreadPool.activeThreadCount());
        m_DicomThreadPool.waitForDone();
        LogUtil::Debug(CODE_LOCATION, "All threads done", m_DicomThreadPool.activeThreadCount());
    }
}

void DicomExecutor::Wait()
{
    QMutexLocker lock(&m_MutexTaskRun);
    m_ConditionRun.wait(&m_MutexTaskRun);
}

void DicomExecutor::WakeOne()
{
    QMutexLocker lock(&m_MutexTaskRun);
    m_ConditionRun.wakeOne();
}

void DicomExecutor::WakeAll()
{
    QMutexLocker lock(&m_MutexTaskRun);
    m_ConditionRun.wakeAll();
}

bool DicomExecutor::IsRunning()
{
    int atomicInt = m_AtomicRuning.loadAcquire();
    return atomicInt == 1;
}

void DicomExecutor::run()
{

    LogUtil::Debug(CODE_LOCATION, "Thread(%d) %d has started.", QThread::currentThreadId(), m_DicomThreadPool.activeThreadCount());
    while (this->IsRunning())
    {
        QSharedPointer<DicomTaskBase> pTask;
        this->GetTask(pTask);
        if (!pTask.isNull())
        {
            DicomSCUBase *pScuBase = this->GetExecutor(pTask->ExecutorType());
            if (pScuBase == nullptr)
            {
                LogUtil::Warn(CODE_LOCATION, "No DicomSCU type= %d", pTask->ExecutorType());
            }
            else
            {
                pScuBase->ExcuteOperation(pTask);
            }
        }
        else
        {
            this->Wait();
        }
    }
    LogUtil::Debug(CODE_LOCATION, "Thread(%d) %d will exiting.", QThread::currentThreadId());
}
