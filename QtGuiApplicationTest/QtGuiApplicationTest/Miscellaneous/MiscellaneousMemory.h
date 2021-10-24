#ifndef MISCELLANEOUS_MEMORY_H
#define MISCELLANEOUS_MEMORY_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousMemory;};

class MiscellaneousMemory : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousMemory(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousMemory();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnMemoryHeaderTest_clicked();
    void on_btnCaptureDeviceTest_clicked();
    void on_btnEmptyTest_1_clicked();
    void on_btnEmptyTest_2_clicked();

private:
    Ui::MiscellaneousMemory*ui;
};

#endif // MISCELLANEOUS_MEMORY_H
