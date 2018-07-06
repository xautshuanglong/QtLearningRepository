#include "QtGuiApplicationTest.h"

#include <QDebug>
#include <QMouseEvent>
#include <QImageReader>
#include <QImageWriter>

#include <LogUtil.h>

QtGuiApplicationTest::QtGuiApplicationTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(close()));
    QFile mainTabStyle(":/QtGuiApplicationTest/Resources/qss/mainTabWidget.css");
    if (mainTabStyle.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(mainTabStyle.readAll());
        ui.mainTabWidget->setStyleSheet(styleSheet);
        mainTabStyle.close();
    }
    else
    {
        QString error = mainTabStyle.errorString();
    }
}

QtGuiApplicationTest::~QtGuiApplicationTest()
{
    ;
}

bool QtGuiApplicationTest::event(QEvent *event)
{
    //LogUtil::Info(CODE_LOCATION, "Type=%d", event->type());
    return __super::event(event);
}

void QtGuiApplicationTest::mousePressEvent(QMouseEvent *event)
{
    Qt::MouseEventFlags flags = event->flags();
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    QPointF localPos = event->localPos();
    QPointF windowPos = event->windowPos();
    QPointF screenPos = event->screenPos();
    //LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d) LocalPos(%d,%d) WindowPos(%d,%d) ScreenPos(%d,%d)"
    //              " MouseEventFlags=%d Type=%d",
    //              mousePos.x(), mousePos.y(),
    //              localPos.x(), localPos.y(),
    //              windowPos.x(), windowPos.y(),
    //              screenPos.x(), screenPos.y(),
    //              flags, eventType);

    //  Geometry Testing
    //QRect geometry = this->geometry();
    //QRect frame = this->frameGeometry();

    //LogUtil::Debug(CODE_LOCATION, "XY(%d,=%d) Geometry(%d,%d,%d,%d) Frame(%d,%d,%d,%d)",
    //               this->x(), this->y(),
    //               geometry.x(), geometry.y(), geometry.width(), geometry.height(),
    //               frame.x(), frame.y(), frame.width(), frame.height());
}

void QtGuiApplicationTest::mouseReleaseEvent(QMouseEvent *event)
{
    Qt::MouseEventFlags flags = event->flags();
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    QPointF localPos = event->localPos();
    QPointF windowPos = event->windowPos();
    QPointF screenPos = event->screenPos();
    //LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d) LocalPos(%d,%d) WindowPos(%d,%d) ScreenPos(%d,%d)"
    //              " MouseEventFlags=%d Type=%d",
    //              mousePos.x(), mousePos.y(),
    //              localPos.x(), localPos.y(),
    //              windowPos.x(), windowPos.y(),
    //              screenPos.x(), screenPos.y(),
    //              flags, eventType);
}

void QtGuiApplicationTest::mouseDoubleClickEvent(QMouseEvent *event)
{
    Qt::MouseEventFlags flags = event->flags();
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    QPointF localPos = event->localPos();
    QPointF windowPos = event->windowPos();
    QPointF screenPos = event->screenPos();
    LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d) LocalPos(%d,%d) WindowPos(%d,%d) ScreenPos(%d,%d)"
                  " MouseEventFlags=%d Type=%d",
                  mousePos.x(), mousePos.y(),
                  localPos.x(), localPos.y(),
                  windowPos.x(), windowPos.y(),
                  screenPos.x(), screenPos.y(),
                  flags, eventType);

    QByteArrayList formatList = QImageWriter::supportedImageFormats();
    LogUtil::Info(CODE_LOCATION, "ImageWriter supported formats:");
    for (int i = 0; i < formatList.size(); ++i)
    {
        LogUtil::Info(CODE_LOCATION, "WriterFormat[%d] = %s", i, formatList[i].constData());
    }

    formatList = QImageReader::supportedImageFormats();
    LogUtil::Info(CODE_LOCATION, "ImageReader supported formats:");
    for (int i = 0; i < formatList.size(); ++i)
    {
        LogUtil::Info(CODE_LOCATION, "ReaderFormat[%d] = %s", i, formatList[i].constData());
    }
}

void QtGuiApplicationTest::mouseMoveEvent(QMouseEvent *event)
{
    Qt::MouseEventFlags flags = event->flags();
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    QPointF localPos = event->localPos();
    QPointF windowPos = event->windowPos();
    QPointF screenPos = event->screenPos();
    //LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d) LocalPos(%d,%d) WindowPos(%d,%d) ScreenPos(%d,%d)"
    //              " MouseEventFlags=%d Type=%d",
    //              mousePos.x(), mousePos.y(),
    //              localPos.x(), localPos.y(),
    //              windowPos.x(), windowPos.y(),
    //              screenPos.x(), screenPos.y(),
    //              flags, eventType);
    LogUtil::CollectData(CODE_LOCATION, LOG_COLLECTION_TYPE_0, "MousePos %d %d", mousePos.x(), mousePos.y());
}


void QtGuiApplicationTest::wheelEvent(QWheelEvent *event)
{
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d)"
                  " Type=%d",
                  mousePos.x(), mousePos.y(),
                  eventType);
}

void QtGuiApplicationTest::keyPressEvent(QKeyEvent *event)
{
    __super::keyPressEvent(event);
}

void QtGuiApplicationTest::keyReleaseEvent(QKeyEvent *event)
{
    __super::keyReleaseEvent(event);
}


void QtGuiApplicationTest::focusInEvent(QFocusEvent *event)
{
    __super::focusInEvent(event);
}

void QtGuiApplicationTest::focusOutEvent(QFocusEvent *event)
{
    __super::focusOutEvent(event);
}

void QtGuiApplicationTest::enterEvent(QEvent *event)
{
    __super::enterEvent(event);
}

void QtGuiApplicationTest::leaveEvent(QEvent *event)
{
    __super::leaveEvent(event);
}

void QtGuiApplicationTest::paintEvent(QPaintEvent *event)
{
    __super::paintEvent(event);
}

void QtGuiApplicationTest::moveEvent(QMoveEvent *event)
{
    __super::moveEvent(event);
}

void QtGuiApplicationTest::resizeEvent(QResizeEvent *event)
{
    __super::resizeEvent(event);
}

void QtGuiApplicationTest::closeEvent(QCloseEvent *event)
{
    LogUtil::Info(CODE_LOCATION, "Type=%d Enter close event...", event->type());
    __super::closeEvent(event);
}
