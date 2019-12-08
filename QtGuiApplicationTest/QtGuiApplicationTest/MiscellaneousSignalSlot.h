#ifndef MISCELLANEOUS_SIGNAL_SLOT_H
#define MISCELLANEOUS_SIGNAL_SLOT_H

#include "MiscellaneousBase.h"

namespace Ui { class MiscellaneousSignalSlot; };

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
    void SlotMainThreadQbject(const QObject& testObj);
    void SlotMainThreadQbjectPointer(QObject* pTestObj);
    void SlotMainThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj);
    void on_btnEmitSignalMainThread_clicked();
    void on_btnEmitSignalSubThread_clicked();

signals:
    void SignalMainThreadVoid();
    void SignalMainThreadInteger(int testInt);
    void SignalMainThreadIntegerSharedPointer(QSharedPointer<int> pTestInt);
    void SignalMainThreadQbject(const QObject& testObj);
    void SignalMainThreadQbjectPointer(QObject* pTestObj);
    void SignalMainThreadQbjectSharedPointer(QSharedPointer<QObject> pTestObj);

private:
    Ui::MiscellaneousSignalSlot *ui;
    QObject                     *pObjTestSignals;
};

#endif // MISCELLANEOUS_SIGNAL_SLOT_H
