#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "JCB_Logger/LogUtil.h"
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
    QUrl mainWndUrl(QStringLiteral("qrc:/UI/MainWindow.qml"));
    QQmlApplicationEngine engine;
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [mainWndUrl](QObject* obj, const QUrl& objUrl)
        {
            if (!obj && mainWndUrl == objUrl)
            {
                QCoreApplication::exit(-1);
            }
        }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("bindTest", &bindTest);
    engine.load(mainWndUrl);
    if (engine.rootObjects().isEmpty())
    {
        QCoreApplication::exit(-1);
    }

    int appRetValue = app.exec();
    LogUtil::Info(CODE_LOCATION, "================== Application will exiting: %d ==================", appRetValue);
    return appRetValue;
}
