#include "MiscellaneousWinWMI.h"
#include "ui_MiscellaneousWinWMI.h"

#include <windows.h>
#include <tchar.h>

#include "JCB_Logger/LogUtil.h"

MiscellaneousWinWMI::MiscellaneousWinWMI(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousWinWMI())
    , mIsMonitorRegRunning(false)
    , mpRegMonitorThread(nullptr)
{
    ui->setupUi(this);
}

MiscellaneousWinWMI::~MiscellaneousWinWMI()
{
    delete ui;
}

QString MiscellaneousWinWMI::GetTitle()
{
    return QObject::tr("WMI");
}

QString MiscellaneousWinWMI::GetTitleTooltip()
{
    return QObject::tr("How to use WMI to query and monitor event.");
}

MiscellaneousTestGroup MiscellaneousWinWMI::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinWMI::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_WMI;
}

void MiscellaneousWinWMI::on_btnMonitorRegStart_clicked()
{
    int i = 0;
}

void MiscellaneousWinWMI::on_btnMonitorRegStop_clicked()
{
    int i = 0;
}

void MiscellaneousWinWMI::on_btnEventWrite_clicked()
{
    int i = 0;
}

void MiscellaneousWinWMI::on_btnEmptyTest_clicked()
{
    int i = 0;
}
