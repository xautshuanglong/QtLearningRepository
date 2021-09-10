#include "MiscellaneousSignalSlotBinding.h"

#include <QQmlEngine>

#include "JCB_Logger/LogUtil.h"

MiscellaneousSignalSlotBinding::MiscellaneousSignalSlotBinding(QObject *parent /* = Q_NULLPTR */)
    : QObject(parent)
{
    qmlRegisterType<MiscellaneousSignalSlotBinding>("JcbQuickTest", 1, 0, "MiscellaneousSignalSlotBinding");
    // Once this is registered, the type can be used in QML by importing the specified type namespace and version number:
    // import JcbQuickTest 1.0 // import com.mycompany.qmlcomponents 1.0
    // MiscellaneousSignalSlotBinding{
    //     // ...
    // }

    this->connect(this, &MiscellaneousSignalSlotBinding::sigFromMainWinToBindingTest, this, &MiscellaneousSignalSlotBinding::slotFromMainWinToBindingTest, Qt::QueuedConnection);
}

MiscellaneousSignalSlotBinding::~MiscellaneousSignalSlotBinding()
{
}

void MiscellaneousSignalSlotBinding::slotMainWindowCompleted(int intValue, QString strValue)
{
    LogUtil::Debug(CODE_LOCATION, "Main window load completed -> int:%d  string:%s", intValue, strValue.toUtf8().data());
}

void MiscellaneousSignalSlotBinding::slotFromMainWinToBindingTest(int intValue, QString strValue)
{
    LogUtil::Debug(CODE_LOCATION, "receive signal from main window  -> int:%d  string:%s", intValue, strValue.toUtf8().data());
}
