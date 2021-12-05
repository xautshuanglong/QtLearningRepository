#include "MiscellaneousEventTrace.h"
#include "ui_MiscellaneousEventTrace.h"

#include "JCB_Logger/LogUtil.h"

MiscellaneousEventTrace::MiscellaneousEventTrace(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousEventTrace())
{
    ui->setupUi(this);
}

MiscellaneousEventTrace::~MiscellaneousEventTrace()
{
    delete ui;
}

QString MiscellaneousEventTrace::GetTitle()
{
    return QObject::tr("Event Trace");
}

QString MiscellaneousEventTrace::GetTitleTooltip()
{
    return QObject::tr("How to use event trace for windows.");
}

MiscellaneousTestGroup MiscellaneousEventTrace::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousEventTrace::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_EVent_Trace;
}

void MiscellaneousEventTrace::on_btnEventSubscript_clicked()
{
    int i = 0;
}

void MiscellaneousEventTrace::on_btnEventExport_clicked()
{
    int i = 0;
}

void MiscellaneousEventTrace::on_btnEventWrite_clicked()
{
    int i = 0;
}

void MiscellaneousEventTrace::on_btnEmptyTest_clicked()
{
    int i = 0;
}
