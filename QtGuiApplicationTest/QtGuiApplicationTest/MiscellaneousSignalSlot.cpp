#include "MiscellaneousSignalSlot.h"
#include "ui_MiscellaneousSignalSlot.h"

#include <QThread>

MiscellaneousSignalSlot::MiscellaneousSignalSlot(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousSignalSlot())
    , pObjTestSignals(new QObject(this))
{
    ui->setupUi(this);

    pObjTestSignals->setObjectName("test_signal_qobject_pointer");
    pObjTestSignals->setProperty("INT", 110);
    pObjTestSignals->setProperty("STRING", "Hello World. qobject_pointer");

    bool resultFlag = false;
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadVoid()), this, SLOT(SlotMainThreadVoid()));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadInteger(int)), this, SLOT(SlotMainThreadInteger(int)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadIntegerSharedPointer(QSharedPointer<int>)), this, SLOT(SlotMainThreadIntegerSharedPointer(QSharedPointer<int>)));
    // QObject Testing
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQbject(const QObject&)), this, SLOT(SlotMainThreadQbject(const QObject&)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQbjectPointer(QObject*)), this, SLOT(SlotMainThreadQbjectPointer(QObject*)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQbjectSharedPointer(QSharedPointer<QObject>)), this, SLOT(SlotMainThreadQbjectSharedPointer(QSharedPointer<QObject>)));
    // ExtendQObject Testing
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadExtendQbject(const ExtendQObject&)), this, SLOT(SlotMainThreadExtendQbject(const ExtendQObject&)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadExtendQbjectPointer(ExtendQObject*)), this, SLOT(SlotMainThreadExtendQbjectPointer(ExtendQObject*)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject>)), this, SLOT(SlotMainThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject>)));

    // 子线程信号槽测试（队列化连接方式）
    SignalTestWorker *pSignalWorker = new SignalTestWorker(this);
    QThread *pWorkerThread = new QThread(this);
    QObject *parentBefore = pSignalWorker->parent();
    pSignalWorker->moveToThread(pWorkerThread);
    QObject *parentAfter = pSignalWorker->parent();
    // QObject Testing
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQbject(const QObject&)), pSignalWorker, SLOT(SlotSubThreadQbject(const QObject&)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQbjectPointer(QObject*)), pSignalWorker, SLOT(SlotSubThreadQbjectPointer(QObject*)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQbjectSharedPointer(QSharedPointer<QObject>)), pSignalWorker, SLOT(SlotSubThreadQbjectSharedPointer(QSharedPointer<QObject>)));
    // ExtendQObject Testing
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadExtendQbject(const ExtendQObject&)), pSignalWorker, SLOT(SlotSubThreadExtendQbject(const ExtendQObject&)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadExtendQbjectPointer(ExtendQObject*)), pSignalWorker, SLOT(SlotSubThreadExtendQbjectPointer(ExtendQObject*)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject>)), pSignalWorker, SLOT(SlotSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject>)));
}

MiscellaneousSignalSlot::~MiscellaneousSignalSlot()
{
    delete ui;
}

void MiscellaneousSignalSlot::SlotMainThreadVoid()
{
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadInteger(int testInt)
{
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadIntegerSharedPointer(QSharedPointer<int> pTestInt)
{
    int testInt = *pTestInt;
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadQbject(const QObject& testObj)
{
    int testInt = testObj.property("INT").toInt();
    QString objName = testObj.objectName();
    QString testString = testObj.property("STRING").toString();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadQbjectPointer(QObject* pTestObj)
{
    int testInt = pTestObj->property("INT").toInt();
    QString objName = pTestObj->objectName();
    QString testString = pTestObj->property("STRING").toString();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj)
{
    int testInt = pTestObj->property("INT").toInt();
    QString objName = pTestObj->objectName();
    QString testString = pTestObj->property("STRING").toString();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadExtendQbject(const ExtendQObject& testObj)
{
    int testInt = testObj.property("INT").toInt();
    QString objName = testObj.objectName();
    QString testString = testObj.property("STRING").toString();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadExtendQbjectPointer(ExtendQObject* pTestObj)
{
    int testInt = pTestObj->property("INT").toInt();
    QString objName = pTestObj->objectName();
    QString testString = pTestObj->property("STRING").toString();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject> pTestObj)
{
    int testInt = pTestObj->property("INT").toInt();
    QString objName = pTestObj->objectName();
    QString testString = pTestObj->property("STRING").toString();
    int i = 0;
}

void MiscellaneousSignalSlot::on_btnEmitSignalMainThread_clicked()
{
    emit SignalMainThreadVoid();
    emit SignalMainThreadInteger(110);
    emit SignalMainThreadIntegerSharedPointer(QSharedPointer<int>(new int(119)));

    // QObject Testing
    QObject testObject;
    testObject.setObjectName("test_signal_qobject");
    testObject.setProperty("INT", 110);
    testObject.setProperty("STRING", "Hello World. qobject");
    emit SignalMainThreadQbject(testObject);
    emit SignalMainThreadQbjectPointer(pObjTestSignals);
    QSharedPointer<QObject> spTestObj(new QObject(this));
    spTestObj->setObjectName("test_signal_qobject_pointer_shared");
    spTestObj->setProperty("INT", 110);
    spTestObj->setProperty("STRING", "Hello world, qobject_shared_pointer!");
    emit SignalMainThreadQbjectSharedPointer(spTestObj);

    // ExtendQObject Testing
    ExtendQObject testExtendObject;
    testExtendObject.setObjectName("test_signal_extend_qobject");
    testExtendObject.setProperty("INT", 110);
    testExtendObject.setProperty("STRING", "Hello World. extend_qobject");
    emit SignalMainThreadExtendQbject(testExtendObject);
    ExtendQObject *pExtendObject = new ExtendQObject(this);
    pExtendObject->setObjectName("test_signal_extend_qobject_pointer");
    pExtendObject->setProperty("INT", 110);
    pExtendObject->setProperty("STRING", "Hello World. extend_qobject_pointer");
    emit SignalMainThreadExtendQbjectPointer(pExtendObject);
    QSharedPointer<ExtendQObject> spTestExtendObj(new ExtendQObject(this));
    spTestExtendObj->setObjectName("test_signal_extend_qobject_pointer_shared");
    spTestExtendObj->setProperty("INT", 110);
    spTestExtendObj->setProperty("STRING", "Hello world, extend_qobject_shared_pointer!");
    emit SignalMainThreadExtendQbjectSharedPointer(spTestExtendObj);
}

void MiscellaneousSignalSlot::on_btnEmitSignalSubThread_clicked()
{
    // QObject Testing
    QObject testObject;
    testObject.setObjectName("test_signal_qobject_sub_thread");
    testObject.setProperty("INT", 110);
    testObject.setProperty("STRING", "Hello World. qobject_sub_thread");
    emit SignalSubThreadQbject(testObject);
    emit SignalSubThreadQbjectPointer(pObjTestSignals);
    QSharedPointer<QObject> spTestObj(new QObject(this));
    spTestObj->setObjectName("test_signal_qobject_pointer_shared_sub_thread");
    spTestObj->setProperty("INT", 110);
    spTestObj->setProperty("STRING", "Hello world, qobject_pointer_shared_sub_thread!");
    emit SignalSubThreadQbjectSharedPointer(spTestObj);

    // ExtendQObject Testing
    ExtendQObject testExtendObject;
    testExtendObject.setObjectName("test_signal_extend_qobject_sub_thread");
    testExtendObject.setProperty("INT", 110);
    testExtendObject.setProperty("STRING", "Hello World. extend_qobject_sub_thread");
    emit SignalSubThreadExtendQbject(testExtendObject);
    ExtendQObject *pExtendObject = new ExtendQObject(this);
    pExtendObject->setObjectName("test_signal_extend_qobject_pointer_sub_thread");
    pExtendObject->setProperty("INT", 110);
    pExtendObject->setProperty("STRING", "Hello World. extend_qobject_pointer_sub_thread");
    emit SignalSubThreadExtendQbjectPointer(pExtendObject);
    QSharedPointer<ExtendQObject> spTestExtendObj(new ExtendQObject(this));
    spTestExtendObj->setObjectName("test_signal_extend_qobject_pointer_shared_sub_thread");
    spTestExtendObj->setProperty("INT", 110);
    spTestExtendObj->setProperty("STRING", "Hello world, extend_qobject_pointer_shared_sub_thread!");
    emit SignalSubThreadExtendQbjectSharedPointer(spTestExtendObj);
}

QString MiscellaneousSignalSlot::GetTitle()
{
    return QObject::tr("Signal Slot");
}

QString MiscellaneousSignalSlot::GetTitleTooltip()
{
    return QObject::tr("Signal slot pass self defined type.");
}

MiscellaneousTestGroup MiscellaneousSignalSlot::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousSignalSlot::GetItemID()
{
    return MiscellaneousTestItem::QT_Signal_Slot;
}

void SignalTestWorker::SlotSubThreadQbject(const QObject& testObj)
{
    int testInt = testObj.property("INT").toInt();
    QString objName = testObj.objectName();
    QString testString = testObj.property("STRING").toString();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadQbjectPointer(QObject* pTestObj)
{
    int testInt = pTestObj->property("INT").toInt();
    QString objName = pTestObj->objectName();
    QString testString = pTestObj->property("STRING").toString();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj)
{
    int testInt = pTestObj->property("INT").toInt();
    QString objName = pTestObj->objectName();
    QString testString = pTestObj->property("STRING").toString();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadExtendQbject(const ExtendQObject& testObj)
{
    int testInt = testObj.property("INT").toInt();
    QString objName = testObj.objectName();
    QString testString = testObj.property("STRING").toString();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadExtendQbjectPointer(ExtendQObject* pTestObj)
{
    int testInt = pTestObj->property("INT").toInt();
    QString objName = pTestObj->objectName();
    QString testString = pTestObj->property("STRING").toString();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject> pTestObj)
{
    int testInt = pTestObj->property("INT").toInt();
    QString objName = pTestObj->objectName();
    QString testString = pTestObj->property("STRING").toString();
    int i = 0;
}
