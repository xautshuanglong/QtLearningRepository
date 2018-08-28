#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include <LogUtil.h>
#include "TestManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG);
#else
    LogUtil::Init(LOG_LEVEL_INFO);
#endif

    LogUtil::Debug(CODE_LOCATION, "-------------- Start testing --------------");

    TestManager::Instance()->Enter();
    int retVlue = app.exec();
    TestManager::Instance()->Exit();

    return retVlue;
}
