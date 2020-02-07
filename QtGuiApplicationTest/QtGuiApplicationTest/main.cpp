#include "MyApplication.h"
#include "MyMainWindow.h"

// QT Headers
#include <QApplication>
#include <QDebug>

// Google ProtoBuf
#include <google/protobuf/stubs/common.h>

// VLD
//#define VLD_FORCE_ENABLE
//#include <vld/vld.h>

// Self Headers
#include <LogUtil.h>

#include "NotifyWidget.h"

void ExitCallback()
{
    int i = 0;
}

int main(int argc, char *argv[])
{
    atexit(ExitCallback);
    MyApplication app(argc, argv);

#ifdef QT_DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG, true);
#else
    LogUtil::Init(LOG_LEVEL_INFO, false);
#endif

    LogUtil::Info(CODE_LOCATION, "================== Application started ==================");

    MyMainWindow mainWindow;
    mainWindow.show();
    NotifyWidget::CreateInstance();

    int appRetValue = app.exec();

    NotifyWidget::DestroyInstance();
    google::protobuf::ShutdownProtobufLibrary();

    return appRetValue;
}
