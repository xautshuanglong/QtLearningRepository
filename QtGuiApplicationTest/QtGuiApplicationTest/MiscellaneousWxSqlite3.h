#ifndef MISCELLANEOUS_WXSQLITE3_H
#define MISCELLANEOUS_WXSQLITE3_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousWxSqlite3.h"

class MiscellaneousWxSqlite3 : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWxSqlite3(QWidget *parent = 0);
    ~MiscellaneousWxSqlite3();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;


private:
    Ui::MiscellaneousWxSqlite3 ui;
};

#endif // MISCELLANEOUS_WXSQLITE3_H
