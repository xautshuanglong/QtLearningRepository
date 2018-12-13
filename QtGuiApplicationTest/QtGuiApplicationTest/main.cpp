#include "MyApplication.h"
#include "MyMainWindow.h"

// QT Headers
#include <QtWidgets/QApplication>
#include <QDebug>

// VLD
//#define VLD_FORCE_ENABLE
//#include <vld/vld.h>

// Self Headers
#include <LogUtil.h>

int main(int argc, char *argv[])
{
    MyApplication app(argc, argv);

#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG);
#else
    LogUtil::Init(LOG_LEVEL_INFO);
#endif

    MyMainWindow mainWindow;
    mainWindow.show();

    LogUtil::Info(CODE_LOCATION, "================== Application started ==================");

    return app.exec();
}
