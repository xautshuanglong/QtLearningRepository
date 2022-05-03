#include "MiscellaneousWinSystemInfo.h"

#include <windows.h>

#include "JCB_Logger/LogUtil.h"

MiscellaneousWinSystemInfo::MiscellaneousWinSystemInfo(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousWinSystemInfo::~MiscellaneousWinSystemInfo()
{
}

QString MiscellaneousWinSystemInfo::GetTitle()
{
    return QObject::tr("System Information");
}

QString MiscellaneousWinSystemInfo::GetTitleTooltip()
{
    return QObject::tr("How to use windows API get system information.");
}

MiscellaneousTestGroup MiscellaneousWinSystemInfo::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinSystemInfo::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_SystemInfo;
}

void MiscellaneousWinSystemInfo::on_btnSystemInfo_clicked()
{
    int i = 0;
}

void MiscellaneousWinSystemInfo::on_btnTotalMemory_clicked()
{
    int i = 0;
}

void MiscellaneousWinSystemInfo::on_btnEmptyTest1_clicked()
{
    int i = 0;
}

void MiscellaneousWinSystemInfo::on_btnEmptyTest2_clicked()
{
    int i = 0;
}
