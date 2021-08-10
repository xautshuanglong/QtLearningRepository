#include "MiscellaneousSignalSlotBinding.h"

#include "LogUtil.h"

MiscellaneousSignalSlotBinding::MiscellaneousSignalSlotBinding(QObject *parent /* = Q_NULLPTR */)
    : QObject(parent)
{
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
