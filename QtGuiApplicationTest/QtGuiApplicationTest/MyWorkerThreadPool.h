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
/*  后台工作者线程池（线程数量固定为 CPU 内核数）                          */
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

    void WaitForTask();      // 等待任务就绪
    void NotifyOneWorker();  // 唤醒一个工作者
    void NotifyAllWorker();  // 唤醒所有工作者

private:
    QMutex                  mMutexTask;     // 任务队列互斥访问
    QMutex                  mMutexWork;     // 配合等待条件使用
    QWaitCondition          mConditionWork; // 工作者处理任务条件（没有任务是阻塞线程）
    QAtomicInteger<qint32>  mAtomicRuning;  // 指示线程池是否已启动（工作中）
    SPWorkerTaskList        mListTask;
    SPWorkerThreadList      mListThread;
};

#endif // MY_WORKER_THREAD_POOL_H
