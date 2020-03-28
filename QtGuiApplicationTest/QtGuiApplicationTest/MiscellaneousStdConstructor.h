#ifndef MISCELLANEOUS_STD_CONSTRUCTOR_H
#define MISCELLANEOUS_STD_CONSTRUCTOR_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousStdConstructor.h"

class MiscellaneousStdConstructor : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousStdConstructor(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousStdConstructor();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnConstructorCommon_clicked();
    void on_btnConstructorMove_clicked();
    void on_btnConstructorCopy_clicked();
    void on_btnConstructorAssign_clicked();

private:
    Ui::MiscellaneousStdConstructor ui;
};

#endif // MISCELLANEOUS_STD_CONSTRUCTOR_H
