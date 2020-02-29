#include "MiscellaneousSqlCipher.h"

MiscellaneousSqlCipher::MiscellaneousSqlCipher(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousSqlCipher::~MiscellaneousSqlCipher()
{
}

QString MiscellaneousSqlCipher::GetTitle()
{
    return QObject::tr("SqlCipher");
}

QString MiscellaneousSqlCipher::GetTitleTooltip()
{
    return QObject::tr("How to use SqlCipher for secure sqlite.");
}

MiscellaneousTestGroup MiscellaneousSqlCipher::GetGroupID()
{
    return MiscellaneousTestGroup::Database;
}

MiscellaneousTestItem MiscellaneousSqlCipher::GetItemID()
{
    return MiscellaneousTestItem::Database_Sqlite_SqlCipher;
}

void MiscellaneousSqlCipher::on_btnInsert_clicked()
{
    int i = 0;
}

void MiscellaneousSqlCipher::on_btnDelete_clicked()
{
    int i = 0;
}

void MiscellaneousSqlCipher::on_btnUpdate_clicked()
{
    int i = 0;
}
void MiscellaneousSqlCipher::on_btnSelect_clicked()
{
    int i = 0;
}
