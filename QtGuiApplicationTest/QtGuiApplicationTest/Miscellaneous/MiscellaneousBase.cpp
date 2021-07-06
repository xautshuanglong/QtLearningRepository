#include "MiscellaneousBase.h"

MiscellaneousBase::MiscellaneousBase(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
}

MiscellaneousBase::~MiscellaneousBase()
{
}

QString MiscellaneousBase::GetTitle()
{
    return QObject::tr("MiscellaneousTestTitle");
}

QString MiscellaneousBase::GetTitleTooltip()
{
    return QObject::tr("Tooltip to describe what will be tested.");
}

MiscellaneousTestGroup MiscellaneousBase::GetGroupID()
{
    return MiscellaneousTestGroup::Max_Size;
}

MiscellaneousTestItem MiscellaneousBase::GetItemID()
{
    return MiscellaneousTestItem::Max_Size;
}
