#include "MyMainWindow.h"

#include <QDebug>
#include <QMouseEvent>
#include <QImageReader>
#include <QImageWriter>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windowsx.h>
#endif

#include <LogUtil.h>
#include "TitleBar.h"

MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //Qt::WindowFlags oldFlags = windowFlags();
    //setWindowFlags(oldFlags | Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    TitleBar *pTitleBar = new TitleBar(this);

    QFile mainTabStyle(":/QtGuiApplicationTest/Resources/qss/mainTabWidget.qss");
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
    //LogUtil::CollectData(CODE_LOCATION, LOG_COLLECTION_TYPE_0, "MousePos %d %d", mousePos.x(), mousePos.y());
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

bool MyMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    MSG *pMsg = static_cast<MSG*>(message);
    switch (pMsg->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(pMsg->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(pMsg->lParam) - this->geometry().y();

        if (this->childAt(nX, nY) != NULL)
            return QWidget::nativeEvent(eventType, message, result);

        *result = HTCAPTION;

        if ((nX > 0) && (nX < 5))
            *result = HTLEFT;

        if ((nX > this->width() - 5) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < 5))
            *result = HTTOP;

        if ((nY > this->height() - 5) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < 5) && (nY > 0)
            && (nY < 5))
            *result = HTTOPLEFT;

        if ((nX > this->width() - 5) && (nX < this->width())
            && (nY > 0) && (nY < 5))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < 5)
            && (nY > this->height() - 5) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - 5) && (nX < this->width())
            && (nY > this->height() - 5) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;
        return true;
    }
    default:
        break;
    }
#endif

    return QWidget::nativeEvent(eventType, message, result);
}
