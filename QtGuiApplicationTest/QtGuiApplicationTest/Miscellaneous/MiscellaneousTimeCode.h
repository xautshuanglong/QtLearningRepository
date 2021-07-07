#ifndef MISCELLANEOUS_TIME_CODE_H
#define MISCELLANEOUS_TIME_CODE_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousTimeCode;};

class MiscellaneousTimeCode : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousTimeCode(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousTimeCode();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnTransferTest_clicked();
    void on_btnEventMapTest_clicked();
    void on_btnEmptyTest_1_clicked();
    void on_btnEmptyTest_2_clicked();

private:
    Ui::MiscellaneousTimeCode *ui;
};

#endif // MISCELLANEOUS_TIME_CODE_H
