#include "MyWorkerThreadPool.h"

#include <LogUtil.h>

MyWorkerThreadPool::MyWorkerThreadPool()
    : mAtomicRuning(0)
{
}

MyWorkerThreadPool::~MyWorkerThreadPool()
{
    this->Stop();
}

void MyWorkerThreadPool::Start()
{
    if (mAtomicRuning.testAndSetOrdered(0, 1))
    {
        int cpuCount = QThread::idealThreadCount();
        for (int i = 0; i < cpuCount; ++i)
        {
            QSharedPointer<MyWorkerThread> pTempThread(new MyWorkerThread(this));
            mListThread.push_back(pTempThread);
            pTempThread->start();
        }
    }
}

void MyWorkerThreadPool::Stop()
{
    if (mAtomicRuning.testAndSetOrdered(1, 0))
    {
        mConditionWork.wakeAll();
        int threadSize = mListThread.size();
        for (int i = 0; i < threadSize; ++i)
        {
            mListThread[i]->wait(ULONG_MAX); // 等待线程退出
        }
        QMutexLocker lock(&mMutexTask);
        mListThread.clear();
    }
}

void MyWorkerThreadPool::AddTask(SPWorkerTask pTask)
{
    QMutexLocker lock(&mMutexTask);
    mListTask.push_back(pTask);
    mConditionWork.wakeOne();
    LogUtil::Debug(CODE_LOCATION, "TaskList size:%d", mListTask.size());
}

void MyWorkerThreadPool::RemoveTask(SPWorkerTask pTask)
{
    QMutexLocker lock(&mMutexTask);
    mListTask.removeAll(pTask);
    LogUtil::Debug(CODE_LOCATION, "TaskList size:%d", mListTask.size());
}

bool MyWorkerThreadPool::GetTask(SPWorkerTask &pOutTask)
{
    bool retValue = false;
    QMutexLocker lock(&mMutexTask);
    SPWorkerTask pTempTask = Q_NULLPTR;
    int taskSize = mListTask.size();
    if (taskSize > 0)
    {
        pTempTask = mListTask.first();
        mListTask.erase(mListTask.begin());
        pOutTask = pTempTask;
        retValue = true;
    }
    LogUtil::Debug(CODE_LOCATION, "TaskList size:%d", taskSize);

    return retValue;
}

bool MyWorkerThreadPool::IsRunning()
{
    int atomicInt = mAtomicRuning.loadAcquire();
    return atomicInt == 1;
}

void MyWorkerThreadPool::WaitForTask()
{
    mMutexWork.lock();
    mConditionWork.wait(&mMutexWork);
}

void MyWorkerThreadPool::NotifyOneWorker()
{
    mConditionWork.wakeOne();
}

void MyWorkerThreadPool::NotifyAllWorker()
{
    mConditionWork.wakeAll();
}
