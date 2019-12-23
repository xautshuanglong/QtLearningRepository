#include "MiscellaneousQObjectCast.h"
#include "ui_MiscellaneousQObjectCast.h"

MiscellaneousQObjectCast::MiscellaneousQObjectCast(QWidget *parent)
	: MiscellaneousBase(parent)
{
	ui = new Ui::MiscellaneousQObjectCast();
	ui->setupUi(this);
}

MiscellaneousQObjectCast::~MiscellaneousQObjectCast()
{
	delete ui;
}

void MiscellaneousQObjectCast::on_btnSameModule_clicked()
{
	int i = 0;
}

void MiscellaneousQObjectCast::on_btnDllQObject_clicked()
{
	int i = 0;
}

void MiscellaneousQObjectCast::on_btnDllQObjectExtra_clicked()
{
	int i = 0;
}

QString MiscellaneousQObjectCast::GetTitle()
{
	return QObject::tr("Widget QObject Cast");
}

QString MiscellaneousQObjectCast::GetTitleTooltip()
{
	return QObject::tr("How to cast QObject between parent and child.");
}

MiscellaneousTestGroup MiscellaneousQObjectCast::GetGroupID()
{
	return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQObjectCast::GetItemID()
{
	return MiscellaneousTestItem::QT_Object_Cast;
}
