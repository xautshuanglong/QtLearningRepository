#include "MiscellaneousSignalSlot.h"
#include "ui_MiscellaneousSignalSlot.h"

MiscellaneousSignalSlot::MiscellaneousSignalSlot(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousSignalSlot())
{
    ui->setupUi(this);
}

MiscellaneousSignalSlot::~MiscellaneousSignalSlot()
{
    delete ui;
}

void MiscellaneousSignalSlot::on_btnEmitSignalMainThread_clicked()
{
    int i = 0;
}

void MiscellaneousSignalSlot::on_btnEmitSignalSubThread_clicked()
{
    int i = 0;
}

QString MiscellaneousSignalSlot::GetTitle()
{
    return QObject::tr("Signal Slot");
}

QString MiscellaneousSignalSlot::GetTitleTooltip()
{
    return QObject::tr("Signal slot pass self defined type.");
}

MiscellaneousTestGroup MiscellaneousSignalSlot::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousSignalSlot::GetItemID()
{
    return MiscellaneousTestItem::QT_Signal_Slot;
}
