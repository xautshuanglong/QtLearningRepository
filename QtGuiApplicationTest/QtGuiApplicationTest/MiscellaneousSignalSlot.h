#ifndef MISCELLANEOUS_SIGNAL_SLOT_H
#define MISCELLANEOUS_SIGNAL_SLOT_H

#include "MiscellaneousBase.h"

namespace Ui { class MiscellaneousSignalSlot; };

class SelfDefinedClass
{
public:
    SelfDefinedClass(){}
    ~SelfDefinedClass(){}
};

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
    // QObject Testing
    void SlotSubThreadQbject(const QObject& testObj);
    void SlotSubThreadQbjectPointer(QObject* pTestObj);
    void SlotSubThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj);
    // ExtendQObject Testing
    void SlotSubThreadExtendQbject(const ExtendQObject& testObj);
    void SlotSubThreadExtendQbjectPointer(ExtendQObject* pTestObj);
    void SlotSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject> pTestObj);

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

    void on_btnEmitSignalMainThread_clicked();
    void on_btnEmitSignalSubThread_clicked();

signals:
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
    // QObject Testing
    void SignalSubThreadQbject(const QObject& testObj);
    void SignalSubThreadQbjectPointer(QObject* pTestObj);
    void SignalSubThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj);
    // ExtendQObject Testing
    void SignalSubThreadExtendQbject(const ExtendQObject& testObj);
    void SignalSubThreadExtendQbjectPointer(ExtendQObject* pTestObj);
    void SignalSubThreadExtendQbjectSharedPointer(QSharedPointer<ExtendQObject> pTestObj);

private:
    Ui::MiscellaneousSignalSlot *ui;
    QObject                     *pObjTestSignals;
};

#endif // MISCELLANEOUS_SIGNAL_SLOT_H
