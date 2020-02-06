#include "MiscellaneousPostgresql.h"

// libpq/libpqxx Headers
/************************************************************************/
/* postgresql 数据库相关操作测试                                          */
/*     libpq   c   语言接口                                              */
/*     libpqxx c++ 语言接口                                              */
/************************************************************************/

MiscellaneousPostgresql::MiscellaneousPostgresql(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousPostgresql::~MiscellaneousPostgresql()
{
}

QString MiscellaneousPostgresql::GetTitle()
{
    return QObject::tr("Postgresql");
}

QString MiscellaneousPostgresql::GetTitleTooltip()
{
    return QObject::tr("How to use libpq/libpqxx for postgresql.");
}

MiscellaneousTestGroup MiscellaneousPostgresql::GetGroupID()
{
    return MiscellaneousTestGroup::Database;
}

MiscellaneousTestItem MiscellaneousPostgresql::GetItemID()
{
    return MiscellaneousTestItem::Database_Sqlite_Postgresql;
}

void MiscellaneousPostgresql::on_btnLibpqInsert_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqDelete_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqUpdate_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqSelect_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqxxInsert_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqxxDelete_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqxxUpdate_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqxxSelect_clicked()
{
    int i = 0;
}
