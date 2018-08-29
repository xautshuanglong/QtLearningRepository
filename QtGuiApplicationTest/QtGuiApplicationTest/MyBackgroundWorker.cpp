#include "MyBackgroundWorker.h"

#include <QThread>
#include <QThreadPool>
#include <LogUtil.h>

MyBackgroundWorker::MyBackgroundWorker()
{
    LogUtil::Debug(CODE_LOCATION, "0x%08X is constructing ...", this);
}

MyBackgroundWorker::~MyBackgroundWorker()
{
    LogUtil::Debug(CODE_LOCATION, "0x%08X is destructing ...", this);
}

void MyBackgroundWorker::run()
{
    int i = 0;

    while (i < 100)
    {
        ++i;
        LogUtil::Debug(CODE_LOCATION, "%s 0x%08x is running  =====",
                       QThread::currentThread()->objectName().toStdString().c_str(),
                       this);
        QThread::msleep(1);
    }

    int ActiveThreadCount = QThreadPool::globalInstance()->activeThreadCount();
    LogUtil::Debug(CODE_LOCATION, "----------> ActiveThreadCount=%d", ActiveThreadCount);
}
