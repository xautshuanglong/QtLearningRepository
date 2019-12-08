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
    void on_btnEmitSignalMainThread_clicked();
    void on_btnEmitSignalSubThread_clicked();

private:
    Ui::MiscellaneousSignalSlot *ui;
};

#endif // MISCELLANEOUS_SIGNAL_SLOT_H
