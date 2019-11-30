#include "MiscellaneousImageQImage.h"
#include "ui_MiscellaneousImageQImage.h"

MiscellaneousImageQImage::MiscellaneousImageQImage(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousImageQImage())
{
    ui->setupUi(this);
}

MiscellaneousImageQImage::~MiscellaneousImageQImage()
{
    delete ui;
}

QString MiscellaneousImageQImage::GetTitle()
{
    return QObject::tr("Image QImage");
}

QString MiscellaneousImageQImage::GetTitleTooltip()
{
    return QObject::tr("QImage operations: copy assign and others.");
}

MiscellaneousTestGroup MiscellaneousImageQImage::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousImageQImage::GetItemID()
{
    return MiscellaneousTestItem::QT_Image_QImage;
}

