#include "MiscellaneousWxSqlite3.h"

MiscellaneousWxSqlite3::MiscellaneousWxSqlite3(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousWxSqlite3::~MiscellaneousWxSqlite3()
{

}

QString MiscellaneousWxSqlite3::GetTitle()
{
    return QObject::tr("wxSqlite3");
}

QString MiscellaneousWxSqlite3::GetTitleTooltip()
{
    return QObject::tr("How to use wxSqlite3 for secure sqlite.");
}

MiscellaneousTestGroup MiscellaneousWxSqlite3::GetGroupID()
{
    return MiscellaneousTestGroup::Database;
}

MiscellaneousTestItem MiscellaneousWxSqlite3::GetItemID()
{
    return MiscellaneousTestItem::Sqlite_wxSqlite3;
}
