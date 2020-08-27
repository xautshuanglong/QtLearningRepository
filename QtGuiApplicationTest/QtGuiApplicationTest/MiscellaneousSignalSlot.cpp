#include "MiscellaneousSignalSlot.h"
#include "ui_MiscellaneousSignalSlot.h"

#include <QThread>
#include <QMetaType>

MiscellaneousSignalSlot::MiscellaneousSignalSlot(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousSignalSlot())
    , pObjTestSignals(new QObject(this))
    , pWorkerThread(new QThread(this))
{
    ui->setupUi(this);

    int metaID = qRegisterMetaType<SelfDefinedClass>();
    metaID = qRegisterMetaType<QSharedPointer<SelfDefinedClass>>();
    //metaID = qRegisterMetaType<QSharedPointer<ExtendQObject>>(); // 此处声明元类型，导致释放崩溃，同一个 QObject 不能属于两个线程

    pObjTestSignals->setObjectName("test_signal_qobject_pointer");
    pObjTestSignals->setProperty("INT", 110);
    pObjTestSignals->setProperty("STRING", "Hello World. qobject_pointer");

    pSelfDefinedClass = new SelfDefinedClass();
    pSelfDefinedClass->SetValue(119);
    pSelfDefinedClass->SetName("test_signal_custom_class_pointer");

    bool resultFlag = false;
    // primitive data type testing
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
    // SelfDefinedClass Testing
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadSelfDefinedClass(const SelfDefinedClass&)), this, SLOT(SlotMainThreadSelfDefinedClass(const SelfDefinedClass&)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadSelfDefinedClassPointer(SelfDefinedClass*)), this, SLOT(SlotMainThreadSelfDefinedClassPointer(SelfDefinedClass*)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass>)), this, SLOT(SlotMainThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass>)));
    // Wrapped with QVariant
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQVariant(const QVariant)), this, SLOT(SlotMainThreadQVariant(const QVariant)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQVariantPointer(const QVariant*)), this, SLOT(SlotMainThreadQVariantPointer(const QVariant*)));
    resultFlag = this->connect(this, SIGNAL(SignalMainThreadQVariantList(const QVariantList)), this, SLOT(SlotMainThreadQVariantList(const QVariantList)));

    // 子线程信号槽测试（队列化连接方式）
    SignalTestWorker *pSignalWorker = new SignalTestWorker();
    pWorkerThread->setObjectName("TestSignalThread");
    QObject *parentBefore = pSignalWorker->parent();
    QString treadBefore = pSignalWorker->thread()->objectName();
    pSignalWorker->moveToThread(pWorkerThread);
    QObject *parentAfter = pSignalWorker->parent();
    QString threadAfter = pSignalWorker->thread()->objectName();
    // primitive data type testing
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadVoid()), pSignalWorker, SLOT(SlotSubThreadVoid()));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadInteger(int)), pSignalWorker, SLOT(SlotSubThreadInteger(int)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadIntegerSharedPointer(QSharedPointer<int>)), pSignalWorker, SLOT(SlotSubThreadIntegerSharedPointer(QSharedPointer<int>)));
    // QObject Testing
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQbject(const QObject&)), pSignalWorker, SLOT(SlotSubThreadQbject(const QObject&)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQbjectPointer(QObject*)), pSignalWorker, SLOT(SlotSubThreadQbjectPointer(QObject*)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQbjectSharedPointer(QSharedPointer<QObject>)), pSignalWorker, SLOT(SlotSubThreadQbjectSharedPointer(QSharedPointer<QObject>)));
    // ExtendQObject Testing
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadExtendQbject(const ExtendQObject&)), pSignalWorker, SLOT(SlotSubThreadExtendQbject(const ExtendQObject&)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadExtendQbjectPointer(ExtendQObject*)), pSignalWorker, SLOT(SlotSubThreadExtendQbjectPointer(ExtendQObject*)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject>)), pSignalWorker, SLOT(SlotSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject>)));
    // SelfDefinedClass Testing
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadSelfDefinedClass(const SelfDefinedClass&)), pSignalWorker, SLOT(SlotSubThreadSelfDefinedClass(const SelfDefinedClass&)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadSelfDefinedClassPointer(SelfDefinedClass*)), pSignalWorker, SLOT(SlotSubThreadSelfDefinedClassPointer(SelfDefinedClass*)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass>)), pSignalWorker, SLOT(SlotSubThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass>)));
    // Wrapped with QVariant
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQVariant(const QVariant)), pSignalWorker, SLOT(SlotSubThreadQVariant(const QVariant)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQVariantPointer(const QVariant*)), pSignalWorker, SLOT(SlotSubThreadQVariantPointer(const QVariant*)));
    resultFlag = this->connect(this, SIGNAL(SignalSubThreadQVariantList(const QVariantList)), pSignalWorker, SLOT(SlotSubThreadQVariantList(const QVariantList)));

    pWorkerThread->start();
}

MiscellaneousSignalSlot::~MiscellaneousSignalSlot()
{
    delete ui;

    if (pWorkerThread)
    {
        pWorkerThread->quit();
        pWorkerThread->wait(5000);
        delete pWorkerThread;
        pWorkerThread = Q_NULLPTR;
    }
    if (pSelfDefinedClass)
    {
        delete pSelfDefinedClass;
        pSelfDefinedClass = Q_NULLPTR;
    }
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

void MiscellaneousSignalSlot::SlotMainThreadSelfDefinedClass(const SelfDefinedClass& testObj)
{
    int testInt = testObj.GetValue();
    QString objName = testObj.GetName();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadSelfDefinedClassPointer(SelfDefinedClass* pTestObj)
{
    int testInt = pTestObj->GetValue();
    QString objName = pTestObj->GetName();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass> pTestObj)
{
    int testInt = pTestObj->GetValue();
    QString objName = pTestObj->GetName();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadQVariant(const QVariant variantData)
{
    ToBeWrappedDaata testData = variantData.value<ToBeWrappedDaata>();
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadQVariantPointer(const QVariant* pVariantData)
{
    ToBeWrappedDaata testData = pVariantData->value<ToBeWrappedDaata>();
    delete pVariantData;
    int i = 0;
}

void MiscellaneousSignalSlot::SlotMainThreadQVariantList(const QVariantList variantList)
{
    for each (QVariant variantData in variantList)
    {
        ToBeWrappedDaata testData = variantData.value<ToBeWrappedDaata>();
        int i = 0;
    }
    int i = 0;
}

void MiscellaneousSignalSlot::on_btnEmitSignalMainThread_clicked()
{
    // primitive data type testing
    emit SignalMainThreadVoid();
    emit SignalMainThreadInteger(110);
    emit SignalMainThreadIntegerSharedPointer(QSharedPointer<int>(new int(110)));

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
    // primitive data type testing
    emit SignalSubThreadVoid();
    emit SignalSubThreadInteger(110);
    emit SignalSubThreadIntegerSharedPointer(QSharedPointer<int>(new int(110)));

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

void MiscellaneousSignalSlot::on_btnEmitSignalMainThreadCustomClass_clicked()
{
    SelfDefinedClass testSelfDefinedClass;
    testSelfDefinedClass.SetValue(119);
    testSelfDefinedClass.SetName("test_signal_custom_class");
    emit SignalMainThreadSelfDefinedClass(testSelfDefinedClass);
    emit SignalMainThreadSelfDefinedClassPointer(pSelfDefinedClass);
    QSharedPointer<SelfDefinedClass> spTestSelfDefinedClass(new SelfDefinedClass());
    spTestSelfDefinedClass->SetValue(119);
    spTestSelfDefinedClass->SetName("test_signal_custom_class_pointer_shared");
    emit SignalMainThreadSelfDefinedClassSharedPointer(spTestSelfDefinedClass);
}

void MiscellaneousSignalSlot::on_btnEmitSignalSubThreadCustomClass_clicked()
{
    SelfDefinedClass testSelfDefinedClass;
    testSelfDefinedClass.SetValue(119);
    testSelfDefinedClass.SetName("test_signal_custom_class");
    emit SignalSubThreadSelfDefinedClass(testSelfDefinedClass);
    emit SignalSubThreadSelfDefinedClassPointer(pSelfDefinedClass);
    QSharedPointer<SelfDefinedClass> spTestSelfDefinedClass(new SelfDefinedClass());
    spTestSelfDefinedClass->SetValue(119);
    spTestSelfDefinedClass->SetName("test_signal_custom_class_pointer_shared");
    emit SignalSubThreadSelfDefinedClassSharedPointer(spTestSelfDefinedClass);
}

void MiscellaneousSignalSlot::on_btnEmitSignalMainThreadQVariant_clicked()
{
    // QVariant Testing
    ToBeWrappedDaata testData;
    testData.intValue = 110;
    testData.strValue = "Testing_110";
    testData.intList.append(1);
    testData.intList.append(1);
    testData.intList.append(0);
    QVariant testVariant;
    testVariant.setValue(testData);
    emit SignalMainThreadQVariant(testVariant);

    // QVariant Pointer Testing
    ToBeWrappedDaata testData_2;
    testData_2.intValue = 119;
    testData_2.strValue = "Testing_119";
    testData_2.intList.append(1);
    testData_2.intList.append(1);
    testData_2.intList.append(9);
    QVariant *pTestVariant = new QVariant();
    pTestVariant->setValue(testData_2);

    QVariantList testVariantList;
    testVariantList.append(testVariant);
    testVariantList.append(*pTestVariant);
    emit SignalMainThreadQVariantList(testVariantList);

    // 槽函数中会将 pTestVariant 指针删除
    emit SignalMainThreadQVariantPointer(pTestVariant);
}

void MiscellaneousSignalSlot::on_btnEmitSignalSubThreadQVariant_clicked()
{
    // QVariant Testing
    ToBeWrappedDaata testData;
    testData.intValue = 110;
    testData.strValue = "Testing_110";
    testData.intList.append(1);
    testData.intList.append(1);
    testData.intList.append(0);
    QVariant testVariant;
    testVariant.setValue(testData);
    emit SignalSubThreadQVariant(testVariant);

    // QVariant Pointer Testing
    ToBeWrappedDaata testData_2;
    testData_2.intValue = 119;
    testData_2.strValue = "Testing_119";
    testData_2.intList.append(1);
    testData_2.intList.append(1);
    testData_2.intList.append(9);
    QVariant *pTestVariant = new QVariant();
    pTestVariant->setValue(testData_2);

    QVariantList testVariantList;
    testVariantList.append(testVariant);
    testVariantList.append(*pTestVariant);
    emit SignalSubThreadQVariantList(testVariantList);

    // 槽函数中会将 pTestVariant 指针删除
    emit SignalSubThreadQVariantPointer(pTestVariant);
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

void SignalTestWorker::SlotSubThreadVoid()
{
    int i = 0;
}

void SignalTestWorker::SlotSubThreadInteger(int testInt)
{
    int i = 0;
}

void SignalTestWorker::SlotSubThreadIntegerSharedPointer(QSharedPointer<int> pTestInt)
{
    int testInt = *pTestInt;
    int i = 0;
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

void SignalTestWorker::SlotSubThreadSelfDefinedClass(const SelfDefinedClass& testObj)
{
    int testInt = testObj.GetValue();
    QString objName = testObj.GetName();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadSelfDefinedClassPointer(SelfDefinedClass* pTestObj)
{
    int testInt = pTestObj->GetValue();
    QString objName = pTestObj->GetName();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass> pTestObj)
{
    int testInt = pTestObj->GetValue();
    QString objName = pTestObj->GetName();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadQVariant(const QVariant variantData)
{
    ToBeWrappedDaata testData = variantData.value<ToBeWrappedDaata>();
    int i = 0;
}

void SignalTestWorker::SlotSubThreadQVariantPointer(const QVariant* pVariantData)
{
    ToBeWrappedDaata testData = pVariantData->value<ToBeWrappedDaata>();
    delete pVariantData;
    int i = 0;
}

void SignalTestWorker::SlotSubThreadQVariantList(const QVariantList variantList)
{
    for each (QVariant variantData in variantList)
    {
        ToBeWrappedDaata testData = variantData.value<ToBeWrappedDaata>();
        int i = 0;
    }
    int i = 0;
}
