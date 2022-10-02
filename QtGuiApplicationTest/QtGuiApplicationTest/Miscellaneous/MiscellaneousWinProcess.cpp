#include "MiscellaneousWinProcess.h"
#include "ui_MiscellaneousWinProcess.h"

#include <windows.h>
#include <tchar.h>

#include "JCB_Logger/LogUtil.h"

MiscellaneousWinProcess::MiscellaneousWinProcess(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousWinProcess())
{
    ui->setupUi(this);
}

MiscellaneousWinProcess::~MiscellaneousWinProcess()
{
    delete ui;
}

QString MiscellaneousWinProcess::GetTitle()
{
    return QObject::tr("Process");
}

QString MiscellaneousWinProcess::GetTitleTooltip()
{
    return QObject::tr("How to operate process and some testing base on process.");
}

MiscellaneousTestGroup MiscellaneousWinProcess::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinProcess::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Process;
}

void MiscellaneousWinProcess::on_btnEnumProcess_clicked()
{
    int i = 0;
}

void MiscellaneousWinProcess::on_btnEnumModule_clicked()
{
    int i = 0;
}
