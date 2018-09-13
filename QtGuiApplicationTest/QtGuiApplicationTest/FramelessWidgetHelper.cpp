#include "FramelessWidgetHelper.h"

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QRect>
#include <QStyle>
#include <QToolBar>
#include <QToolButton>
#include <QMainWindow>
#include <QRubberBand>

#include <LogUtil.h>

FramelessWidgetHelper::FramelessWidgetHelper(QWidget *parent /* = Q_NULLPTR */)
    : QObject(parent)
{
}

FramelessWidgetHelper::~FramelessWidgetHelper()
{
}

bool FramelessWidgetHelper::eventFilter(QObject *obj, QEvent *event)
{
    int i = 0;
    switch (event->type())
    {
    case QEvent::MouseMove:
        return HandleEventMouseMove(obj, static_cast<QMouseEvent*>(event));
    case QEvent::HoverMove:
        return HandleEventHoverMove(obj, static_cast<QHoverEvent*>(event));
    case QEvent::MouseButtonPress:
        return HandleEventMouseButtonPress(obj, static_cast<QMouseEvent*>(event));
    case QEvent::MouseButtonRelease:
        return HandleEventMouseButtonRelease(obj, static_cast<QMouseEvent*>(event));
    case QEvent::Leave:
        return HandleEventMouseLeave(obj, static_cast<QMouseEvent*>(event));
    case QEvent::WindowTitleChange:
        return HandleEventWindowTitleChange(obj, event);
    case QEvent::WindowIconChange:
        return HandleEventWindowIconChange(obj, event);
    case QEvent::WindowStateChange:
        return HandleEventWindowStateChange(obj, static_cast<QWindowStateChangeEvent*>(event));
    default:
        return QObject::eventFilter(obj, event);
    }

    //return QObject::eventFilter(obj, event);
}

bool FramelessWidgetHelper::HandleEventMouseMove(QObject *obj, QMouseEvent *event)
{
    if (mbLeftBtnPressed)
    {
        if (mbWidgetResizable && mbOnEdge)
        {
            QPoint globalMousePos = event->globalPos();
            ResizeWindow(globalMousePos);
        }
        else if (mbWidgetMovable)
        {
        }
    }
    //else if (mbWidgetResizable)
    //{
    //    QPoint *pGlobalPos = new QPoint(event->globalPos());
    //    UpdateCursorShape(pGlobalPos);
    //    delete pGlobalPos;
    //}

    return QObject::eventFilter(obj, event);
}

bool FramelessWidgetHelper::HandleEventHoverMove(QObject *obj, QHoverEvent *event)
{
    if (mbWidgetResizable && !mbLeftBtnPressed)
    {
        // 已点击模式下（可能正在拖动）保持光标形态
        // 未点击模式下重新判断鼠标位置并改变光标形态
        QPoint globalMousePos = mpMainWindow->mapToGlobal(event->pos());
        UpdateCursorShape(globalMousePos);
    }
    return QObject::eventFilter(obj, event);
}

bool FramelessWidgetHelper::HandleEventMouseButtonPress(QObject *obj, QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftBtnPressed = true;

        QRect frameRect = mpMainWindow->frameGeometry();
        QPoint globalMousePos = event->globalPos();
        UpdateCursorShape(globalMousePos);

        if (mbOnEdge)
        {
            mpRubberBand->setGeometry(frameRect);
            mpRubberBand->show();
        }
    }
    return QObject::eventFilter(obj, event);
}

bool FramelessWidgetHelper::HandleEventMouseButtonRelease(QObject *obj, QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftBtnPressed = false;
        if (mpRubberBand && mpRubberBand->isVisible())
        {
            mpRubberBand->hide();
            mpMainWindow->setGeometry(mpRubberBand->geometry());
        }
        QPoint globalPos = event->globalPos();
        UpdateCursorShape(globalPos);
    }
    return QObject::eventFilter(obj, event);
}

bool FramelessWidgetHelper::HandleEventMouseLeave(QObject *obj, QMouseEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWidgetHelper::HandleEventWindowTitleChange(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWidgetHelper::HandleEventWindowIconChange(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWidgetHelper::HandleEventWindowStateChange(QObject *obj, QWindowStateChangeEvent *event)
{
    return QObject::eventFilter(obj, event);
}

void FramelessWidgetHelper::ResizeWindow(const QPoint& globalMousePos)
{
    QRect originalWinRect;
    if (mbRubberBandOnResize)
    {
        originalWinRect = mpRubberBand->frameGeometry();
    }
    else
    {
        originalWinRect = mpMainWindow->frameGeometry();
    }

    int left = 0, top = 0, right = 0, bottom = 0;
    originalWinRect.getCoords(&left, &top, &right, &bottom);

    int minWidth = mpMainWindow->minimumWidth();
    int minHeight = mpMainWindow->minimumHeight();

    if (mbOnCornerTopLeft)
    {
        left = globalMousePos.x();
        top = globalMousePos.y();
    }
    else if (mbOnCornerTopRight)
    {
        right = globalMousePos.x();
        top = globalMousePos.y();
    }
    else if (mbOnCornerBottomLeft)
    {
        left = globalMousePos.x();
        bottom = globalMousePos.y();
    }
    else if (mbOnCornerBottomRight)
    {
        right = globalMousePos.x();
        bottom = globalMousePos.y();
    }
    else if (mbOnEdgeLeft)
    {
        left = globalMousePos.x();
    }
    else if (mbOnEdgeRight)
    {
        right = globalMousePos.x();
    }
    else if (mbOnEdgeTop)
    {
        top = globalMousePos.y();
    }
    else if (mbOnEdgeBottom)
    {
        bottom = globalMousePos.y();
    }

    QRect newRect(QPoint(left, top), QPoint(right, bottom));
    if (newRect.isValid())
    {
        if (minWidth > newRect.width())
        {
            if (left != originalWinRect.left())
            {
                newRect.setLeft(originalWinRect.left());
            }
            else
            {
                newRect.setRight(originalWinRect.right());
            }
        }
        if (minHeight > newRect.height())
        {
            if (top != originalWinRect.top())
            {
                newRect.setTop(originalWinRect.top());
            }
            else
            {
                newRect.setBottom(originalWinRect.bottom());
            }
        }

        if (mbRubberBandOnResize)
        {
            mpRubberBand->setGeometry(newRect);
        }
        else
        {
            mpMainWindow->setGeometry(newRect);
        }
    }
}

void FramelessWidgetHelper::CheckCursorPosition(const QPoint& globalMousePos, const QRect& frameRect)
{
    int globalMouseX = globalMousePos.x();
    int globalMouseY = globalMousePos.y();
    int frameLeft = frameRect.left();
    int frameRight = frameRect.right();
    int frameTop = frameRect.top();
    int frameBottom = frameRect.bottom();

    mbOnEdgeLeft = globalMouseX >= frameLeft && globalMouseX <= frameLeft + mnBorderWidth;
    mbOnEdgeRight = globalMouseX <= frameRight && globalMouseX >= frameRight - mnBorderWidth;
    mbOnEdgeTop = globalMouseY >= frameTop && globalMouseY <= frameTop + mnBorderWidth;
    mbOnEdgeBottom = globalMouseY <= frameBottom && globalMouseY >= frameBottom - mnBorderWidth;

    mbOnCornerTopLeft = mbOnEdgeLeft && mbOnEdgeTop;
    mbOnCornerTopRight = mbOnEdgeRight && mbOnEdgeTop;
    mbOnCornerBottomLeft = mbOnEdgeLeft && mbOnEdgeBottom;
    mbOnCornerBottomRight = mbOnEdgeRight && mbOnEdgeBottom;

    mbOnEdge = mbOnEdgeLeft || mbOnEdgeRight || mbOnEdgeTop || mbOnEdgeBottom;
}

void FramelessWidgetHelper::UpdateCursorShape(const QPoint& globalMousePos)
{
    if (mpMainWindow == Q_NULLPTR) return;

    if (mpMainWindow->isFullScreen() || mpMainWindow->isMaximized())
    {
        if (mbCursorShapeChanged)
        {
            mpMainWindow->unsetCursor();
        }
    }

    QRect mainWindowRect = mpMainWindow->frameGeometry();
    CheckCursorPosition(globalMousePos, mainWindowRect);

    if (mbOnCornerTopLeft || mbOnCornerBottomRight)
    {
        mpMainWindow->setCursor(Qt::SizeFDiagCursor);
        mbCursorShapeChanged = true;
    }
    else if (mbOnCornerBottomLeft || mbOnCornerTopRight)
    {
        mpMainWindow->setCursor(Qt::SizeBDiagCursor);
        mbCursorShapeChanged = true;
    }
    else if (mbOnEdgeLeft || mbOnEdgeRight)
    {
        mpMainWindow->setCursor(Qt::SizeHorCursor);
        mbCursorShapeChanged = true;
    }
    else if (mbOnEdgeTop || mbOnEdgeBottom)
    {
        mpMainWindow->setCursor(Qt::SizeVerCursor);
        mbCursorShapeChanged = true;
    }
    else if (mbCursorShapeChanged)
    {
        mpMainWindow->unsetCursor();
        mbCursorShapeChanged = false;
    }
}

QRect FramelessWidgetHelper::GetTitleBarRect()
{
    //return mpFramelessToolBar->rect();
    return QRect();
}
