#pragma once

#include <QWidget>

#include "MiscellaneousDefines.h"

class MiscellaneousBase : public QWidget
{
public:
    MiscellaneousBase(QWidget *parent = Q_NULLPTR);
    virtual ~MiscellaneousBase();

    virtual QString GetTitle();
    virtual QString GetTitleTooltip();
    virtual MiscellaneousTestGroup GetGroupID();
    virtual MiscellaneousTestItem GetItemID();
};
