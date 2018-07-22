#include "MyMainWindow.h"

#include <QDebug>
#include <QMouseEvent>
#include <QImageReader>
#include <QImageWriter>

#include <LogUtil.h>
#include "TitleBar.h"

MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    TitleBar *pTitleBar = new TitleBar(this);

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

MyMainWindow::~MyMainWindow()
{
    ;
}

bool MyMainWindow::event(QEvent *event)
{
    //LogUtil::Info(CODE_LOCATION, "Type=%d", event->type());
    return QMainWindow::event(event);
}

void MyMainWindow::mousePressEvent(QMouseEvent *event)
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

void MyMainWindow::mouseReleaseEvent(QMouseEvent *event)
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

void MyMainWindow::mouseDoubleClickEvent(QMouseEvent *event)
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

void MyMainWindow::mouseMoveEvent(QMouseEvent *event)
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


void MyMainWindow::wheelEvent(QWheelEvent *event)
{
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d)"
                  " Type=%d",
                  mousePos.x(), mousePos.y(),
                  eventType);
}

void MyMainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
}

void MyMainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QMainWindow::keyReleaseEvent(event);
}


void MyMainWindow::focusInEvent(QFocusEvent *event)
{
    QMainWindow::focusInEvent(event);
}

void MyMainWindow::focusOutEvent(QFocusEvent *event)
{
    QMainWindow::focusOutEvent(event);
}

void MyMainWindow::enterEvent(QEvent *event)
{
    QMainWindow::enterEvent(event);
}

void MyMainWindow::leaveEvent(QEvent *event)
{
    QMainWindow::leaveEvent(event);
}

void MyMainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
}

void MyMainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
}

void MyMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}

void MyMainWindow::closeEvent(QCloseEvent *event)
{
    LogUtil::Info(CODE_LOCATION, "Type=%d Enter close event...", event->type());
    QMainWindow::closeEvent(event);
}
