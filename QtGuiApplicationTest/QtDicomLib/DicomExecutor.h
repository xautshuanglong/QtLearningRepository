#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QThreadPool>
#include <QWaitCondition>
#include <QMap>
#include <QMutex>

#include "DicomSCUBase.h"
#include "DicomTaskData.h"

class DicomExecutor : public QObject, public QRunnable
{
public:
    DicomExecutor(QObject *parent=Q_NULLPTR);
    ~DicomExecutor();

    void SetExecutor(EnumExecutorType executorType, DicomSCUBase *pDicomSCU) { m_MapExecutors[executorType] = pDicomSCU; }
    DicomSCUBase* GetExecutor(EnumExecutorType executorType) { return m_MapExecutors[executorType]; }

    void AddTask(QSharedPointer<DicomTaskBase> pTask);
    void GetTask(QSharedPointer<DicomTaskBase>& pTask);

    void Start();
    void Stop();

    void Wait();
    void WakeOne();
    void WakeAll();
    bool IsRunning();

    virtual void run() override; // QRunnable

private:
    QList<QSharedPointer<DicomTaskBase>>  m_TaskList;
    QMap<int, DicomSCUBase*>              m_MapExecutors;
    QAtomicInteger<qint32>                m_AtomicRuning;
    QWaitCondition                        m_ConditionRun;
    QMutex                                m_MutexTaskRun;
    QMutex                                m_MutexTaskList;
    QThreadPool                           m_DicomThreadPool;
};
