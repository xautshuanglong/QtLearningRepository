#include "DicomExecutor.h"

// QT Headers
#include <QMutexLocker>

// Self Headers
#include <LogUtil.h>

DicomExecutor::DicomExecutor(QObject *parent/* =Q_NULLPTR */)
    : QObject(parent)
{
    this->setAutoDelete(false);
    m_DicomThreadPool.setExpiryTimeout(-1); // ÓÀ²»¹ýÆÚ
    m_DicomThreadPool.start(this, QThread::LowPriority);
    m_DicomThreadPool.start(this, QThread::LowPriority);

    LogUtil::Debug(CODE_LOCATION, " Constructed ...");
}

DicomExecutor::~DicomExecutor()
{
    LogUtil::Debug(CODE_LOCATION, " Destructed ...");
}

void DicomExecutor::AddTask(QSharedPointer<DicomTaskBase> pTask)
{
    QMutexLocker locker(&m_MutexTaskList);
    m_TaskList.push_back(pTask);
}

void DicomExecutor::GetTask(QSharedPointer<DicomTaskBase>& pTask)
{
    LogUtil::Debug(CODE_LOCATION, "Active thread count: %d", m_DicomThreadPool.activeThreadCount());

    QMutexLocker locker(&m_MutexTaskList);
    if (!m_TaskList.empty())
    {
        pTask = m_TaskList.front();
    }
}

void DicomExecutor::run()
{
    LogUtil::Debug(CODE_LOCATION, "Thread(%d) %d has started.", QThread::currentThreadId(), m_DicomThreadPool.activeThreadCount());
    int i = 20;
    while (i > 0)
    {
        QThread::msleep(500);
        LogUtil::Debug(CODE_LOCATION, "Thread %d isRunning.", QThread::currentThreadId());
        --i;
    }
    LogUtil::Debug(CODE_LOCATION, "Thread(%d) %d will exiting.", QThread::currentThreadId());
}
