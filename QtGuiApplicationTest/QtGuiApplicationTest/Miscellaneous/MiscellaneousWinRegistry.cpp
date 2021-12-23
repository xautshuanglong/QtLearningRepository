#include "MiscellaneousWinRegistry.h"
#include "ui_MiscellaneousWinRegistry.h"

#include "JCB_Logger/LogUtil.h"

MiscellaneousWinRegistry::MiscellaneousWinRegistry(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousWinRegistry())
{
    ui->setupUi(this);
}

MiscellaneousWinRegistry::~MiscellaneousWinRegistry()
{
    delete ui;
}

QString MiscellaneousWinRegistry::GetTitle()
{
    return QObject::tr("Registry");
}

QString MiscellaneousWinRegistry::GetTitleTooltip()
{
    return QObject::tr("How to operate registry and monitor registry change event.");
}

MiscellaneousTestGroup MiscellaneousWinRegistry::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinRegistry::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Registry;
}

void MiscellaneousWinRegistry::on_btnRegNotifyChange_clicked()
{
    int i = 0;
}

void MiscellaneousWinRegistry::on_btnEventExport_clicked()
{
    int i = 0;
}

void MiscellaneousWinRegistry::on_btnEventWrite_clicked()
{
    int i = 0;
}

void MiscellaneousWinRegistry::on_btnEmptyTest_clicked()
{
    int i = 0;
}
