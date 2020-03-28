#include "MiscellaneousStdConstructor.h"

MiscellaneousStdConstructor::MiscellaneousStdConstructor(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousStdConstructor::~MiscellaneousStdConstructor()
{
}

QString MiscellaneousStdConstructor::GetTitle()
{
    return QObject::tr("C++ Constructor");
}

QString MiscellaneousStdConstructor::GetTitleTooltip()
{
    return QObject::tr("How to us standard cpp constructor, copy assign move.");
}

MiscellaneousTestGroup MiscellaneousStdConstructor::GetGroupID()
{
    return MiscellaneousTestGroup::Standard_Cpp;
}

MiscellaneousTestItem MiscellaneousStdConstructor::GetItemID()
{
    return MiscellaneousTestItem::StdCpp_Constructor;
}

void MiscellaneousStdConstructor::on_btnConstructorCommon_clicked()
{
    int i = 0;
}

void MiscellaneousStdConstructor::on_btnConstructorMove_clicked()
{
    int i = 0;
}

void MiscellaneousStdConstructor::on_btnConstructorCopy_clicked()
{
    int i = 0;
}

void MiscellaneousStdConstructor::on_btnConstructorAssign_clicked()
{
    int i = 0;
}
