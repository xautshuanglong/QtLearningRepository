#pragma once

#include <QObject>
#include <QThreadPool>
#include <QMap>
#include <QMutex>
#include <QSharedPointer>

#include "DicomSCUBase.h"
#include "DicomTaskData.h"

class DicomExecutor;

class DicomExecutor : public QObject, public QRunnable
{
public:
    DicomExecutor(QObject *parent=Q_NULLPTR);
    ~DicomExecutor();

    void SetExecutor(EnumExecutorType executorType, DicomSCUBase *pDicomSCU) { m_MapExecutors[executorType] = pDicomSCU; }
    DicomSCUBase* GetExecutor(EnumExecutorType executorType) { return m_MapExecutors[executorType]; }

    void AddTask(QSharedPointer<DicomTaskBase> pTask);
    void GetTask(QSharedPointer<DicomTaskBase>& pTask);

    virtual void run() override; // QRunnable

private:
    QList<QSharedPointer<DicomTaskBase>>  m_TaskList;
    QMap<int, DicomSCUBase*>              m_MapExecutors;
    QMutex                                m_MutexTaskList;
    QThreadPool                           m_DicomThreadPool;
};
