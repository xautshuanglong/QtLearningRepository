#include <QCoreApplication>

#include <QDebug>
#include <QThread>

#include <LogUtil.h>

#include "TestManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG);
#elif
    MGI::LogUtil::Init(MGI::LOG_LEVEL_INFO);
#endif

    LogUtil::Debug(CODE_LOCATION, "-------------- Start testing --------------");

    TestManager::Instance()->Enter();

    return a.exec();
}
