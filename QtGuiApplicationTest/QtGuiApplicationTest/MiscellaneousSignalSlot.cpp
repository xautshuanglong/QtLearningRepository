#include "MiscellaneousSignalSlot.h"
#include "ui_MiscellaneousSignalSlot.h"

MiscellaneousSignalSlot::MiscellaneousSignalSlot(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousSignalSlot())
    , pObjTestSignals(new QObject(this))
{
    ui->setupUi(this);

    pObjTestSignals->setObjectName("test_signal_pointer");
    pObjTestSignals->setProperty("INT", 110);
    pObjTestSignals->setProperty("STRING", "Hello World.");

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
    testObject.setObjectName("test_signal");
    testObject.setProperty("INT", 110);
    testObject.setProperty("STRING", "Hello World.");
    emit SignalMainThreadQbject(testObject);
    emit SignalMainThreadQbjectPointer(pObjTestSignals);
    QSharedPointer<QObject> spTestObj(new QObject(this));
    spTestObj->setObjectName("test_signal_pointer_shared");
    spTestObj->setProperty("INT", 110);
    spTestObj->setProperty("STRING", "Hello world, shared pointer!");
    emit SignalMainThreadQbjectSharedPointer(spTestObj);

    // ExtendQObject Testing
    ExtendQObject testExtendObject;
    testExtendObject.setObjectName("test_extend_qobject_signal");
    testExtendObject.setProperty("INT", 110);
    testExtendObject.setProperty("STRING", "Hello World. extend qobject");
    emit SignalMainThreadExtendQbject(testExtendObject);
    ExtendQObject *pExtendObject = new ExtendQObject(this);
    pExtendObject->setObjectName("test_extend_qobject_pointer");
    pExtendObject->setProperty("INT", 110);
    pExtendObject->setProperty("STRING", "Hello World. extend qobject pointer");
    emit SignalMainThreadExtendQbjectPointer(pExtendObject);
    QSharedPointer<ExtendQObject> spTestExtendObj(new ExtendQObject(this));
    spTestExtendObj->setObjectName("test_extend_qobject_pointer_shared");
    spTestExtendObj->setProperty("INT", 110);
    spTestExtendObj->setProperty("STRING", "Hello world, extend qobject shared pointer!");
    emit SignalMainThreadExtendQbjectSharedPointer(spTestExtendObj);
}

void MiscellaneousSignalSlot::on_btnEmitSignalSubThread_clicked()
{
    int i = 0;
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
