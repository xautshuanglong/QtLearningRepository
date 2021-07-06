#include "MiscellaneousImageQImage.h"
#include "ui_MiscellaneousImageQImage.h"

// QT Headers
#include <QImage>

#include <LogUtil.h>

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

void MiscellaneousImageQImage::on_btnImgNoMemory_clicked()
{
    QImage image(100, 100, QImage::Format_RGB888);
    image.fill(Qt::white);
    int imgWidth = image.width();
    int imgHeight = image.height();
    for (int row=0; row<imgHeight; ++row)
    {
        for (int col = 0; col < imgWidth; ++col)
        {
            if (row == col)
            {
                image.setPixel(row, col, qRgb(255, 0, 0));
            }
            if (row + col == imgWidth)
            {
                image.setPixel(row, col, qRgb(0, 0, 255));
            }
        }
    }
    ui->lbImgOriginal->setPixmap(QPixmap::fromImage(image));
}

void MiscellaneousImageQImage::on_btnImgNoMemoryCopy_clicked()
{
    QImage image(100, 80, QImage::Format_RGB888);
    int imgWidth = image.width();
    int imgHeight = image.height();
    for (int row = 0; row < imgHeight; ++row)
    {
        for (int col = 0; col < imgWidth; ++col)
        {
            if (row == col)
            {
                image.setPixel(row, col, qRgb(255, 0, 0));
            }
        }
    }
    QImage testImg = image;
    int i = 0;
}

void MiscellaneousImageQImage::on_btnImgNoMemoryAssign_clicked()
{
    int i = 0;
}

void MiscellaneousImageQImage::on_btnImgNewMemory_clicked()
{
    int i = 0;
}

void MiscellaneousImageQImage::on_btnImgNewMemoryCopy_clicked()
{
    int i = 0;
}

void MiscellaneousImageQImage::on_btnImgNewMemoryAssign_clicked()
{
    int i = 0;
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
