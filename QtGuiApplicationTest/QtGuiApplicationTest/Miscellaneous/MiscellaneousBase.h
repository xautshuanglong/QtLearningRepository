#ifndef MISCELLANEOUS_BASE_H
#define MISCELLANEOUS_BASE_H

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

#endif // MISCELLANEOUS_BASE_H
