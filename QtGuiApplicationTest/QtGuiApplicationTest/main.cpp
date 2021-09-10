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
#include "JCB_Logger/LogUtil.h"

#include "NotifyWidget.h"

void ExitCallback()
{
    int i = 0;
}

int main(int argc, char *argv[])
{
    atexit(ExitCallback);

    // 使用 QT 内嵌输入法，需要配合：QtQML QtQuick 使用，独立打包：qml及其子目录Qt,QtQuick,QtQuick.2
    // 有输入焦点时弹出软键盘，失去输入焦点，软键盘自动消失。
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
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
