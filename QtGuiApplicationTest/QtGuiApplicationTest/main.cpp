#include "QtGuiApplicationTest.h"
#include <QtWidgets/QApplication>

#include <QDebug>

#include <LogUtil.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG);
#elif
    LogUtil::Init(LOG_LEVEL_INFO);
#endif

    QtGuiApplicationTest mainWindow;
    mainWindow.show();

    LogUtil::Info(CODE_LOCATION, "================== Application started ==================");

    return app.exec();
}
