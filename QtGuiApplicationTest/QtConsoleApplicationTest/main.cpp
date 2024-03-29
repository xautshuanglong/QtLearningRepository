
// QT Headers
#include <QCoreApplication>
#include <QDebug>
#include <QThread>

// VLD Headers
#include <vld/vld.h>

// Self Headers
#include "JCB_Logger/LogUtil.h"
#include "TestManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG, true);
#else
    LogUtil::Init(LOG_LEVEL_INFO, false);
#endif

    LogUtil::Debug(CODE_LOCATION, "-------------- Start testing --------------");

    TestManager::Instance()->Enter();
    int retVlue = app.exec();
    TestManager::Instance()->Exit();

    return retVlue;
}
