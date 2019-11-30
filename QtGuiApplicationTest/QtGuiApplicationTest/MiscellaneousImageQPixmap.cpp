#include "MiscellaneousImageQPixmap.h"
#include "ui_MiscellaneousImageQPixmap.h"

MiscellaneousImageQPixmap::MiscellaneousImageQPixmap(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousImageQPixmap())
{
    ui->setupUi(this);
}

MiscellaneousImageQPixmap::~MiscellaneousImageQPixmap()
{
    delete ui;
}

QString MiscellaneousImageQPixmap::GetTitle()
{
    return QObject::tr("Image QPixmap");
}

QString MiscellaneousImageQPixmap::GetTitleTooltip()
{
    return QObject::tr("QPixmap operations: copy assign and others.");
}

MiscellaneousTestGroup MiscellaneousImageQPixmap::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousImageQPixmap::GetItemID()
{
    return MiscellaneousTestItem::QT_Image_QPixmap;
}

