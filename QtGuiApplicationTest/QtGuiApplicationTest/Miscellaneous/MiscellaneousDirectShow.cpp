#include "MiscellaneousDirectShow.h"
#include "ui_MiscellaneousDirectShow.h"

#include "LogUtil.h"

MiscellaneousDirectShow::MiscellaneousDirectShow(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousDirectShow())
{
    ui->setupUi(this);
}

MiscellaneousDirectShow::~MiscellaneousDirectShow()
{
    delete ui;
}

QString MiscellaneousDirectShow::GetTitle()
{
    return QObject::tr("Direct Show");
}

QString MiscellaneousDirectShow::GetTitleTooltip()
{
    return QObject::tr("How to us direct show library.");
}

MiscellaneousTestGroup MiscellaneousDirectShow::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousDirectShow::GetItemID()
{
    return MiscellaneousTestItem::Others_DirectShow;
}

void MiscellaneousDirectShow::on_btnEnumDeviceTest_clicked()
{
    int i = 0;
}

void MiscellaneousDirectShow::on_btnCaptureDeviceTest_clicked()
{
    int i = 0;
}

void MiscellaneousDirectShow::on_btnEmptyTest_1_clicked()
{
    int i = 0;
}

void MiscellaneousDirectShow::on_btnEmptyTest_2_clicked()
{
    int i = 0;
}
