#ifndef MY_WORKER_THREAD_POOL_H
#define MY_WORKER_THREAD_POOL_H

#include "IWorkerTask.h"
#include "MyWorkerThread.h"

#include <QSharedPointer>
#include <QList>
#include <QMutex>
#include <QWaitCondition>
#include <QAtomicInteger>

typedef QSharedPointer<IWorkerTask>               SPWorkerTask;
typedef QList<QSharedPointer<IWorkerTask>>        SPWorkerTaskList;
typedef QList<QSharedPointer<MyWorkerThread>>     SPWorkerThreadList;

/************************************************************************/
/*  ��̨�������̳߳أ��߳������̶�Ϊ CPU �ں�����                          */
/************************************************************************/
class MyWorkerThreadPool
{

public:
    MyWorkerThreadPool();
    ~MyWorkerThreadPool();

    void Start();
    void Stop();
    void AddTask(SPWorkerTask pTask);
    void RemoveTask(SPWorkerTask pTask);
    bool GetTask(SPWorkerTask &pOutTask);
    bool IsRunning();

    void WaitForTask();      // �ȴ��������
    void NotifyOneWorker();  // ����һ��������
    void NotifyAllWorker();  // �������й�����

private:
    QMutex                  mMutexTask;     // ������л������
    QMutex                  mMutexWork;     // ��ϵȴ�����ʹ��
    QWaitCondition          mConditionWork; // �����ߴ�������������û�������������̣߳�
    QAtomicInteger<qint32>  mAtomicRuning;  // ָʾ�̳߳��Ƿ��������������У�
    SPWorkerTaskList        mListTask;
    SPWorkerThreadList      mListThread;
};

#endif // MY_WORKER_THREAD_POOL_H
