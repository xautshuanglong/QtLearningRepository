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
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQbject(const QObject&)), this, SLOT(SlotMainThreadQbject(const QObject&)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQbjectPointer(QObject*)), this, SLOT(SlotMainThreadQbjectPointer(QObject*)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQbjectSharedPointer(QSharedPointer<QObject>)), this, SLOT(SlotMainThreadQbjectSharedPointer(QSharedPointer<QObject>)));
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

void MiscellaneousSignalSlot::on_btnEmitSignalMainThread_clicked()
{
    emit SignalMainThreadVoid();
    emit SignalMainThreadInteger(110);
    emit SignalMainThreadIntegerSharedPointer(QSharedPointer<int>(new int(119)));

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
