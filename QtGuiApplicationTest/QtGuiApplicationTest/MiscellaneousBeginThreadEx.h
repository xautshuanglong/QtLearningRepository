#ifndef MISCELLANEOUS_WINAPI_MULTI_THREAD_H
#define MISCELLANEOUS_WINAPI_MULTI_THREAD_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousBeginThreadEx.h"

class MiscellaneousBeginThreadEx : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousBeginThreadEx(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousBeginThreadEx();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    Ui::MiscellaneousBeginThreadEx ui;
};

#endif // MISCELLANEOUS_WINAPI_MULTI_THREAD_H
