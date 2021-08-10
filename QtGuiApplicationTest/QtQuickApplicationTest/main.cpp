#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "LogUtil.h"
#include "Miscellaneous/MiscellaneousSignalSlotBinding.h"

void ExitCallback()
{
    LogUtil::Info(CODE_LOCATION, "Application Exit .............................."); // 来不及调用???
}

int main(int argc, char *argv[])
{
    atexit(ExitCallback);

#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

#ifdef QT_DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG, true);
#else
    LogUtil::Init(LOG_LEVEL_INFO, false);
#endif
    LogUtil::Info(CODE_LOCATION, "================== Application started ==================");

    MiscellaneousSignalSlotBinding bindTest;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("MiscellaneousSignalSlotBinding", &bindTest);
    engine.load(QUrl(QStringLiteral("qrc:/UI/MainWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int appRetValue = app.exec();
    LogUtil::Info(CODE_LOCATION, "================== Application will exiting: %d ==================", appRetValue);
    return appRetValue;
}
