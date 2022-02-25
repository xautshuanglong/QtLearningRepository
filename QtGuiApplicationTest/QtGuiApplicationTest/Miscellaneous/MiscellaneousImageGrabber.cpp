#include "MiscellaneousImageGrabber.h"
#include "ui_MiscellaneousImageGrabber.h"

// QT Headers
#include <QImage>
#include <QPixmap>
#include <QDateTime>
#include <QThread>
#include <QScreen>
#include <QDesktopWidget>

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
    QDesktopWidget* pDesktop = QApplication::desktop();
    QString filename = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap shotImag = pDesktop->grab();
    shotImag.save(filename);

    QString filename1 = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap pixmap1(pDesktop->size());
    pDesktop->render(&pixmap1);
    pixmap1.save(filename1);

    QString filename2 = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap pixmap2 = QPixmap::grabWindow(pDesktop->screen()->winId());
    pixmap2.save(filename2);
}

void MiscellaneousImageGrabber::on_btnGrabScreen_clicked()
{
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QString filename = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap shotImag = primaryScreen->grabWindow(0);
    shotImag.save(filename);
}

void MiscellaneousImageGrabber::on_btnGrabWidget_clicked()
{
    QWidget* pWidget = this;
    while (pWidget != nullptr)
    {
        QString filename = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
        QPixmap shotImag = pWidget->grab(QRect(QPoint(0, 0), pWidget->size()));
        shotImag.save(filename);
        pWidget = pWidget->parentWidget();
    }
}

void MiscellaneousImageGrabber::on_btnGrabScreenAll_clicked()
{
    int i = 0;
}

void MiscellaneousImageGrabber::on_btnGrabScrollArea_clicked()
{
    int i = 0;
}
