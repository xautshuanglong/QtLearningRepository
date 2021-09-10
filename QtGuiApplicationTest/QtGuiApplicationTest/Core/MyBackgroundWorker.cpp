#include "MyBackgroundWorker.h"

#include <QThread>
#include <QThreadPool>
#include "JCB_Logger/LogUtil.h"

QThreadStorage<QCache<int, int>>  MyBackgroundWorker::m_ThreadLocalTest;

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

    if (!m_ThreadLocalTest.hasLocalData())
    {
        if (!m_ThreadLocalTest.localData().isEmpty())
        {
            m_ThreadLocalTest.localData().clear();
        }
        QCache<int, int> &cacheData = m_ThreadLocalTest.localData();
        cacheData.insert(1, new int(110));
        int i = 0;
    }

    while (i < 100)
    {
        ++i;
        int *pCachedValue = m_ThreadLocalTest.localData().take(1);
        
        LogUtil::Debug(CODE_LOCATION, "%s 0x%08x is running  ===== cached: %d",
                       QThread::currentThread()->objectName().toStdString().c_str(),
                       this, pCachedValue ? *pCachedValue : 0);
        m_ThreadLocalTest.localData().insert(1, new int(++(*pCachedValue)));
        if (pCachedValue)
        {
            delete pCachedValue;
        }
        QThread::msleep(1);
    }

    int ActiveThreadCount = QThreadPool::globalInstance()->activeThreadCount();
    LogUtil::Debug(CODE_LOCATION, "----------> ActiveThreadCount=%d", ActiveThreadCount);
}
