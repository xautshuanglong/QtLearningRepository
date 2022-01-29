#include "MiscellaneousImageGrabber.h"
#include "ui_MiscellaneousImageGrabber.h"

// QT Headers
#include <QImage>

#include "JCB_Logger/LogUtil.h"

MiscellaneousImageGrabber::MiscellaneousImageGrabber(QWidget* parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousImageGrabber())
{
    ui->setupUi(this);
}

MiscellaneousImageGrabber::~MiscellaneousImageGrabber()
{
    delete ui;
}

QString MiscellaneousImageGrabber::GetTitle()
{
    return QObject::tr("Image Grabber");
}

QString MiscellaneousImageGrabber::GetTitleTooltip()
{
    return QObject::tr("Grab window desktop screen and scroll area.");
}

MiscellaneousTestGroup MiscellaneousImageGrabber::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousImageGrabber::GetItemID()
{
    return MiscellaneousTestItem::QT_Image_Grabber;
}

void MiscellaneousImageGrabber::on_btnGrabWindow_clicked()
{
    int i = 0;
}

void MiscellaneousImageGrabber::on_btnGrabDesktop_clicked()
{
    int i = 0;
}

void MiscellaneousImageGrabber::on_btnGrabScreen_clicked()
{
    int i = 0;
}

void MiscellaneousImageGrabber::on_btnGrabWidget_clicked()
{
    int i = 0;
}

void MiscellaneousImageGrabber::on_btnGrabScreenAll_clicked()
{
    int i = 0;
}

void MiscellaneousImageGrabber::on_btnGrabScrollArea_clicked()
{
    int i = 0;
}
