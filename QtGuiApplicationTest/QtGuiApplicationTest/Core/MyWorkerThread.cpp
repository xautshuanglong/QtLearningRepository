#include "MyWorkerThread.h"

#include "JCB_Logger/LogUtil.h"
#include "MyWorkerThreadPool.h"

MyWorkerThread::MyWorkerThread(QObject *parent /* = Q_NULLPTR */)
    : QThread(parent)
{
}

MyWorkerThread::MyWorkerThread(MyWorkerThreadPool *pThreadPool, QObject *parent /* = Q_NULLPTR */)
    : QThread(parent)
    , mpThreadPool(pThreadPool)
{
}

MyWorkerThread::~MyWorkerThread()
{
}

void MyWorkerThread::run()
{
    while (mpThreadPool != Q_NULLPTR && mpThreadPool->IsRunning())
    {
        SPWorkerTask pTask = Q_NULLPTR;
        if (mpThreadPool->GetTask(pTask))
        {
            pTask->DoWorkerTask();
        }
        else
        {
            LogUtil::Debug(CODE_LOCATION, "Before thread wait condition ...");
            mpThreadPool->WaitForTask();
            LogUtil::Debug(CODE_LOCATION, "After thread wait condition ...");
        }
    }
    LogUtil::Debug(CODE_LOCATION, "Thread will exit ...");
}
