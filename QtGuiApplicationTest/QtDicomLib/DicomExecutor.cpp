#include "DicomExecutor.h"

// QT Headers
#include <QMutexLocker>

#include <LogUtil.h>

WorkerThread::WorkerThread(DicomExecutor *pExecutor, QObject *parent /* = Q_NULLPTR */)
    : QThread(parent)
{
    ;
}

WorkerThread::~WorkerThread()
{
    ;
}

DicomExecutor::DicomExecutor(QObject *parent/* =Q_NULLPTR */)
    : QObject(parent)
{
}

DicomExecutor::~DicomExecutor()
{
}

void DicomExecutor::AddTask(QSharedPointer<DicomTaskBase> pTask)
{
    QMutexLocker locker(&m_MutexTaskList);
    m_TaskList.push_back(pTask);
}

void DicomExecutor::GetTask(QSharedPointer<DicomTaskBase>& pTask)
{
    QMutexLocker locker(&m_MutexTaskList);
    if (!m_TaskList.empty())
    {
        pTask = m_TaskList.front();
    }
}

void DicomExecutor::DoTask()
{
    ;
}
