#include "MainTabPageFirst.h"

#include <QImage>
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
    QImage imgTest(256, 256, QImage::Format_RGB16);

    int bitCount = imgTest.bitPlaneCount();

    for (int i = 0; i < 256; ++i)
    {
        for (int j = 0; j < 256; ++j)
        {
            if (i == j || i+j==255 || i==128 || i==255 || i==0 || j==0 || j==128 || j==255)
            {
                imgTest.setPixelColor(i, j, QColor(255, 0, 0, 255));
            }
            else
            {
                imgTest.setPixelColor(i, j, QColor(255, 255, 255, 255));
            }
        }
    }

    ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));

    return QWidget::showEvent(event);
}
