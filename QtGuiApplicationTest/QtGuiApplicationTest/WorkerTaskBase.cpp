#include "WorkerTaskBase.h"

#include <QThread>
#include <LogUtil.h>

WorkerTaskBase::WorkerTaskBase(QObject *parent /* = Q_NULLPTR */)
    : QObject(parent)
{
    LogUtil::Debug(CODE_LOCATION, "this: 0x%08X  constructing", this);
}

WorkerTaskBase::~WorkerTaskBase()
{
    LogUtil::Debug(CODE_LOCATION, "this: 0x%08X  destructing", this);
}

void WorkerTaskBase::DoWorkerTask()
{
    int nCount = 0;
    while (nCount < 100)
    {
        ++nCount;
        LogUtil::Debug(CODE_LOCATION, "this: 0x%08X  count=%d", this, nCount);
        QThread::msleep(100);
    }
}
