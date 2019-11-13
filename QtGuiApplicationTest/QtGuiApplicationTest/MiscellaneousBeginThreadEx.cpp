#include "MiscellaneousBeginThreadEx.h"

MiscellaneousBeginThreadEx::MiscellaneousBeginThreadEx(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousBeginThreadEx::~MiscellaneousBeginThreadEx()
{
}

QString MiscellaneousBeginThreadEx::GetTitle()
{
    return QObject::tr("BeginThreadEx");
}

QString MiscellaneousBeginThreadEx::GetTitleTooltip()
{
    return QObject::tr("How to use windows API create sub thread.");
}

MiscellaneousTestGroup MiscellaneousBeginThreadEx::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Thread;
}

MiscellaneousTestItem MiscellaneousBeginThreadEx::GetItemID()
{
    return MiscellaneousTestItem::Thread_beginthreadex;
}
