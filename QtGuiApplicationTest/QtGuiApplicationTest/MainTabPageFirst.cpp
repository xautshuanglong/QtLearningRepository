#include "MainTabPageFirst.h"

#include <LogUtil.h>

MainTabPageFirst::MainTabPageFirst(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.imgTitleValue->setText("image_filename_test");
}

MainTabPageFirst::~MainTabPageFirst()
{
}

bool MainTabPageFirst::event(QEvent *event)
{
    LogUtil::Debug(CODE_LOCATION, "eventType: %d", event->type());
    switch (event->type())
    {
    case QEvent::Show:
        break;
    default:
        break;
    }
    return QWidget::event(event);
}

void MainTabPageFirst::showEvent(QShowEvent *event)
{
    QImage imgTest("E:\\Preparation\\DICOM\\DICOM-Logo.jpg");

    QPixmap imgContent(512, 512);

    ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));

    return QWidget::showEvent(event);
}
