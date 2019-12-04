#include "MiscellaneousStdSharedPointer.h"
#include "ui_MiscellaneousStdSharedPointer.h"

MiscellaneousStdSharedPointer::MiscellaneousStdSharedPointer(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousStdSharedPointer())
{
    ui->setupUi(this);
}

MiscellaneousStdSharedPointer::~MiscellaneousStdSharedPointer()
{
    delete ui;
}

QString MiscellaneousStdSharedPointer::GetTitle()
{
    return QObject::tr("Shared Pointer");
}

QString MiscellaneousStdSharedPointer::GetTitleTooltip()
{
    return QObject::tr("How to us standard cpp shared pointer.");
}

MiscellaneousTestGroup MiscellaneousStdSharedPointer::GetGroupID()
{
    return MiscellaneousTestGroup::Standard_Cpp;
}

MiscellaneousTestItem MiscellaneousStdSharedPointer::GetItemID()
{
    return MiscellaneousTestItem::StdCpp_Shared_Pointer;
}
