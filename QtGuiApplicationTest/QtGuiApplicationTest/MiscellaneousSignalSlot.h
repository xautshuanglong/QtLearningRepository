#ifndef MISCELLANEOUS_SIGNAL_SLOT_H
#define MISCELLANEOUS_SIGNAL_SLOT_H

#include "MiscellaneousBase.h"

#include <QVariant>

namespace Ui { class MiscellaneousSignalSlot; };

class SelfDefinedClass
{
public:
    explicit SelfDefinedClass()
        : m_intValue(0)
    {}
    SelfDefinedClass(const SelfDefinedClass& other)
    {
        int i = 0;
    }
    ~SelfDefinedClass(){}

    void SetValue(const int value) { m_intValue = value; }
    int GetValue() { return m_intValue; }
    int GetValue() const { return m_intValue; }

    void SetName(const QString& name) { m_strName = name; }
    QString GetName() { return m_strName; }
    QString GetName() const { return m_strName; }

private:
    int      m_intValue;
    QString  m_strName;
};
Q_DECLARE_METATYPE(SelfDefinedClass)
Q_DECLARE_METATYPE(QSharedPointer<SelfDefinedClass>)

class ExtendQObject : public QObject
{
public:
    explicit ExtendQObject(QObject *parent = Q_NULLPTR)
        : QObject(parent)
        , m_intValue(0)
    {}
    ~ExtendQObject(){}

    void SetValue(const int value){ m_intValue = value; }
    int GetValue(){ return m_intValue; }

private:
    int m_intValue;
};
//Q_DECLARE_METATYPE(QSharedPointer<ExtendQObject>)  // �˴�����Ԫ���ͣ������ͷű�����ͬһ�� QObject �������������߳�


/************************************************************************/
/*  ͨ�� QVariant ��װ�Զ���ṹ�壬�����ź����֮�䴫�����ݡ�             */
/************************************************************************/
typedef struct _ToBeWrappedWithQVariant
{
    int intValue;
    QString strValue;
    QList<int> intList;
} ToBeWrappedDaata;
Q_DECLARE_METATYPE(ToBeWrappedDaata)

class SignalTestWorker : public QObject
{
    Q_OBJECT
public:
    explicit SignalTestWorker(QObject *parent = Q_NULLPTR)
        : QObject(parent)
        , m_intValue(0)
    {}
    ~SignalTestWorker() {}

private slots :
    // primitive data type testing
    void SlotSubThreadVoid();
    void SlotSubThreadInteger(int testInt);
    void SlotSubThreadIntegerSharedPointer(QSharedPointer<int> pTestInt);
    // QObject Testing
    void SlotSubThreadQbject(const QObject& testObj);
    void SlotSubThreadQbjectPointer(QObject* pTestObj);
    void SlotSubThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj);
    // ExtendQObject Testing
    void SlotSubThreadExtendQbject(const ExtendQObject& testObj);
    void SlotSubThreadExtendQbjectPointer(ExtendQObject* pTestObj);
    void SlotSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject> pTestObj);
    // SelfDefinedClass Testing
    void SlotSubThreadSelfDefinedClass(const SelfDefinedClass& testObj);
    void SlotSubThreadSelfDefinedClassPointer(SelfDefinedClass* pTestObj);
    void SlotSubThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass> pTestObj);
    // Wrapped with QVariant
    void SlotSubThreadQVariant(const QVariant variantData);
    void SlotSubThreadQVariantPointer(const QVariant* pVariantData);
    void SlotSubThreadQVariantList(const QVariantList variantList);

private:
    int          m_intValue;
};

class MiscellaneousSignalSlot : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousSignalSlot(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousSignalSlot();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    // primitive data type testing
    void SlotMainThreadVoid();
    void SlotMainThreadInteger(int testInt);
    void SlotMainThreadIntegerSharedPointer(QSharedPointer<int> pTestInt);
    // QObject Testing
    void SlotMainThreadQbject(const QObject& testObj);
    void SlotMainThreadQbjectPointer(QObject* pTestObj);
    void SlotMainThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj);
    // ExtendQObject Testing
    void SlotMainThreadExtendQbject(const ExtendQObject& testObj);
    void SlotMainThreadExtendQbjectPointer(ExtendQObject* pTestObj);
    void SlotMainThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject> pTestObj);
    // SelfDefinedClass Testing
    void SlotMainThreadSelfDefinedClass(const SelfDefinedClass& testObj);
    void SlotMainThreadSelfDefinedClassPointer(SelfDefinedClass* pTestObj);
    void SlotMainThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass> pTestObj);
    // Wrapped with QVariant
    void SlotMainThreadQVariant(const QVariant variantData);
    void SlotMainThreadQVariantPointer(const QVariant* pVariantData);
    void SlotMainThreadQVariantList(const QVariantList variantList);

    void on_btnEmitSignalMainThread_clicked();
    void on_btnEmitSignalSubThread_clicked();
    void on_btnEmitSignalMainThreadCustomClass_clicked();
    void on_btnEmitSignalSubThreadCustomClass_clicked();
    void on_btnEmitSignalMainThreadQVariant_clicked();
    void on_btnEmitSignalSubThreadQVariant_clicked();
    void on_btnAllSignalWatcher_clicked();

signals:
    // primitive data type testing
    void SignalMainThreadVoid();
    void SignalMainThreadInteger(int testInt);
    void SignalMainThreadIntegerSharedPointer(QSharedPointer<int> pTestInt);
    // QObject Testing
    void SignalMainThreadQbject(const QObject& testObj);
    void SignalMainThreadQbjectPointer(QObject* pTestObj);
    void SignalMainThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj);
    // ExtendQObject Testing
    void SignalMainThreadExtendQbject(const ExtendQObject& testObj);
    void SignalMainThreadExtendQbjectPointer(ExtendQObject* pTestObj);
    void SignalMainThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject> pTestObj);
    // SelfDefinedClass Testing
    void SignalMainThreadSelfDefinedClass(const SelfDefinedClass& testObj);
    void SignalMainThreadSelfDefinedClassPointer(SelfDefinedClass* pTestObj);
    void SignalMainThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass> pTestObj);
    // Wrapped with QVariant
    void SignalMainThreadQVariant(const QVariant variantData);
    void SignalMainThreadQVariantPointer(const QVariant* pVariantData);
    void SignalMainThreadQVariantList(const QVariantList variantList);
    // primitive data type testing
    void SignalSubThreadVoid();
    void SignalSubThreadInteger(int testInt);
    void SignalSubThreadIntegerSharedPointer(QSharedPointer<int> pTestInt);
    // QObject Testing
    void SignalSubThreadQbject(const QObject& testObj);
    void SignalSubThreadQbjectPointer(QObject* pTestObj);
    void SignalSubThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj);
    // ExtendQObject Testing
    void SignalSubThreadExtendQbject(const ExtendQObject& testObj);
    void SignalSubThreadExtendQbjectPointer(ExtendQObject* pTestObj);
    void SignalSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject> pTestObj);
    // SelfDefinedClass Testing
    void SignalSubThreadSelfDefinedClass(const SelfDefinedClass& testObj);
    void SignalSubThreadSelfDefinedClassPointer(SelfDefinedClass* pTestObj);
    void SignalSubThreadSelfDefinedClassSharedPointer(QSharedPointer<SelfDefinedClass> pTestObj);
    // Wrapped with QVariant
    void SignalSubThreadQVariant(const QVariant variantData);
    void SignalSubThreadQVariantPointer(const QVariant* pVariantData);
    void SignalSubThreadQVariantList(const QVariantList variantList);

private:
    Ui::MiscellaneousSignalSlot *ui;
    QThread                     *pWorkerThread;
    QObject                     *pObjTestSignals;
    SelfDefinedClass            *pSelfDefinedClass;
};

#endif // MISCELLANEOUS_SIGNAL_SLOT_H
