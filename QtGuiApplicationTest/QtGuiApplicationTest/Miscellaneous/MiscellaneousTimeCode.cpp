#include "MiscellaneousTimeCode.h"
#include "ui_MiscellaneousTimeCode.h"

#include "LogUtil.h"

MiscellaneousTimeCode::MiscellaneousTimeCode(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousTimeCode())
{
    ui->setupUi(this);
}

MiscellaneousTimeCode::~MiscellaneousTimeCode()
{
    delete ui;
}

QString MiscellaneousTimeCode::GetTitle()
{
    return QObject::tr("Time Code");
}

QString MiscellaneousTimeCode::GetTitleTooltip()
{
    return QObject::tr("How to us time code MTC/LTC.");
}

MiscellaneousTestGroup MiscellaneousTimeCode::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousTimeCode::GetItemID()
{
    return MiscellaneousTestItem::Others_TimeCode;
}

void MiscellaneousTimeCode::on_btnTransferTest_clicked()
{
    int i = 0;
}

void MiscellaneousTimeCode::on_btnEventMapTest_clicked()
{
    int i = 0;
}

void MiscellaneousTimeCode::on_btnEmptyTest_1_clicked()
{
    int i = 0;
}

void MiscellaneousTimeCode::on_btnEmptyTest_2_clicked()
{
    int i = 0;
}
