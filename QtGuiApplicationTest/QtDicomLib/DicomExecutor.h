#pragma once

#include <QObject>
#include <QThread>
#include <QMap>
#include <QMutex>
#include <QSharedPointer>

#include "DicomSCUBase.h"
#include "DicomTaskData.h"

class WorkerThread : public QThread
{
public:
    WorkerThread(DicomExecutor *pExecutor, QObject *parent = Q_NULLPTR);
    ~WorkerThread();
};

class DicomExecutor : public QObject
{
public:
    DicomExecutor(QObject *parent=Q_NULLPTR);
    ~DicomExecutor();

    void SetExecutor(EnumExecutorType executorType, DicomSCUBase *pDicomSCU) { m_MapExecutors[executorType] = pDicomSCU; }
    DicomSCUBase* GetExecutor(EnumExecutorType executorType) { return m_MapExecutors[executorType]; }

    void AddTask(QSharedPointer<DicomTaskBase> pTask);
    void GetTask(QSharedPointer<DicomTaskBase>& pTask);

    void DoTask();

private:
    QList<QSharedPointer<DicomTaskBase>>  m_TaskList;
    QMap<int, DicomSCUBase*>              m_MapExecutors;
    QMutex                                m_MutexTaskList;
};
