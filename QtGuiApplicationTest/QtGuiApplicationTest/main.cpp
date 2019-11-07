#include "MyApplication.h"
#include "MyMainWindow.h"

// QT Headers
#include <QtWidgets/QApplication>
#include <QDebug>

// Google ProtoBuf
#include <google/protobuf/stubs/common.h>

// VLD
//#define VLD_FORCE_ENABLE
//#include <vld/vld.h>

// Self Headers
#include <LogUtil.h>

#include "NotifyWidget.h"

int main(int argc, char *argv[])
{
    MyApplication app(argc, argv);

#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG);
#else
    LogUtil::Init(LOG_LEVEL_INFO);
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
