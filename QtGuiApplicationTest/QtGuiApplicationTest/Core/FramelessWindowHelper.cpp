#include "FramelessWindowHelper.h"

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

#include "JCB_Logger/LogUtil.h"

FramelessWindowHelper::FramelessWindowHelper(QWidget* parent)
    : QObject(parent)
    , mpMainWindow(Q_NULLPTR)
    , mpRubberBand(Q_NULLPTR)
    , mnBorderWidth(5)
{
    mpFramelessToolBar = new FramelessWindowToolBar(parent);
    mpFramelessToolBar->setObjectName("winTitleBar");

    mpMainWindow = (QMainWindow *)parent->window();
    mpRubberBand = new QRubberBand(QRubberBand::Rectangle);

    parent->installEventFilter(this);
}

FramelessWindowHelper::~FramelessWindowHelper()
{
    if (mpRubberBand != Q_NULLPTR)
    {
        delete mpRubberBand;
        mpRubberBand = Q_NULLPTR;
    }
}

bool FramelessWindowHelper::eventFilter(QObject *obj, QEvent *event)
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

bool FramelessWindowHelper::HandleEventMouseMove(QObject *obj, QMouseEvent *event)
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

bool FramelessWindowHelper::HandleEventHoverMove(QObject *obj, QHoverEvent *event)
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

bool FramelessWindowHelper::HandleEventMouseButtonPress(QObject *obj, QMouseEvent *event)
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

bool FramelessWindowHelper::HandleEventMouseButtonRelease(QObject *obj, QMouseEvent *event)
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

bool FramelessWindowHelper::HandleEventMouseLeave(QObject *obj, QMouseEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWindowHelper::HandleEventWindowTitleChange(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWindowHelper::HandleEventWindowIconChange(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWindowHelper::HandleEventWindowStateChange(QObject *obj, QWindowStateChangeEvent *event)
{
    return QObject::eventFilter(obj, event);
}

void FramelessWindowHelper::ResizeWindow(const QPoint& globalMousePos)
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

void FramelessWindowHelper::CheckCursorPosition(const QPoint& globalMousePos, const QRect& frameRect)
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

void FramelessWindowHelper::UpdateCursorShape(const QPoint& globalMousePos)
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

QRect FramelessWindowHelper::GetTitleBarRect()
{
    return mpFramelessToolBar->rect();
}


/*=========================== FramelessWindowToolBar ===========================*/
FramelessWindowToolBar::FramelessWindowToolBar(QWidget *parent /* = Q_NULLPTR */)
    : QToolBar(parent)
    , mnBorderSize(5)
{
    Q_ASSERT(parent != Q_NULLPTR);

    mpPointMoveStart = new QPoint();
    mpRectNormalWindow = new QRect();

    mpMainWindow = (QMainWindow *)parent->window();
    Qt::WindowFlags winFlags = mpMainWindow->windowFlags();
    mpMainWindow->setWindowFlags(winFlags | Qt::FramelessWindowHint);
    mpMainWindow->addToolBar(this);
    mpMainWindow->installEventFilter(this);

    mpLabelIcon = new QLabel(this);
    mpLabelIcon->setFixedSize(mpMainWindow->iconSize());
    mpLabelIcon->setScaledContents(true);
    mpLabelIcon->setPixmap(mpMainWindow->windowIcon().pixmap(mpLabelIcon->size()));
    this->addWidget(mpLabelIcon);

    mpLabelTitle = new QLabel(this);
    mpLabelTitle->setText(mpMainWindow->windowTitle());
    mpLabelTitle->setStyleSheet("padding-left:5px");
    this->addWidget(mpLabelTitle);

    mpToolBarSeat = new QWidget(this);
    mpToolBarSeat->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->addWidget(mpToolBarSeat);

    mpToolBtnMin = new QToolButton(this);
    mpToolBtnMax = new QToolButton(this);
    mpToolBtnClose = new QToolButton(this);

    mpToolBtnMax->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
    mpToolBtnMin->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarMinButton));
    mpToolBtnClose->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarCloseButton));

    mpToolBtnMax->setToolTip("Maximize");
    mpToolBtnMin->setToolTip("Minimize");
    mpToolBtnClose->setToolTip("Close");

    this->addWidget(mpToolBtnMin);
    this->addWidget(mpToolBtnMax);
    this->addWidget(mpToolBtnClose);

    this->connect(mpToolBtnMax, SIGNAL(clicked()), this, SLOT(SlotMaximizeButtonClicked()));
    this->connect(mpToolBtnMin, SIGNAL(clicked()), mpMainWindow, SLOT(showMinimized()));
    this->connect(mpToolBtnClose, SIGNAL(clicked()), mpMainWindow, SLOT(close()));

    this->setMovable(false);
    this->setContextMenuPolicy(Qt::NoContextMenu);
    parent->setContextMenuPolicy(Qt::NoContextMenu);
}

FramelessWindowToolBar::~FramelessWindowToolBar()
{
    delete mpPointMoveStart;
    delete mpRectNormalWindow;
}

void FramelessWindowToolBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        SlotMaximizeButtonClicked();
        mbLeftDoublePressed = true;
    }
    event->accept();
    //QToolBar::mouseDoubleClickEvent(event);
}

void FramelessWindowToolBar::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton==event->button() && !mbLeftDoublePressed)
    {
        mbLeftPressed = true;
        *mpPointMoveStart = event->globalPos();

        QPoint *pMousePos = new QPoint(event->pos());
        mbCursorOnEdge = this->IsCursorOnEdge(pMousePos);
        delete pMousePos;
    }
    QToolBar::mousePressEvent(event);
}

void FramelessWindowToolBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        mbLeftPressed = false;
        mbLeftDoublePressed = false;
        mbCursorOnEdge = false;
    }
    event->accept();
    QToolBar::mouseReleaseEvent(event);
}

void FramelessWindowToolBar::mouseMoveEvent(QMouseEvent *event)
{
     if (mbLeftPressed)
     {
         if (mpMainWindow->isMaximized())
         {
             QPoint curMousePos = event->globalPos();
             QPoint maxWindowPos = mpMainWindow->pos();
             QSize maxWinSize = mpMainWindow->size();
             float xPercent = (curMousePos.x() - maxWindowPos.x()) * 1.0f / maxWinSize.width();
             QPoint newNormalPos;
             newNormalPos.setX(mpRectNormalWindow->left() + mpRectNormalWindow->width() * xPercent);
             newNormalPos.setY(mpRectNormalWindow->top() + curMousePos.y() - maxWindowPos.y());
             QPoint movePoint = curMousePos - newNormalPos;
             *mpPointMoveStart = curMousePos;
 
             mpMainWindow->showNormal(); // 当为最大化时，还原为正常模式
             mpRectNormalWindow->moveTopLeft(movePoint);
             mpMainWindow->setGeometry(*mpRectNormalWindow);
         }
         else if (!mbCursorOnEdge)
         {
             QPoint movePoint = event->globalPos() - *mpPointMoveStart;
             QPoint widgetPos = mpMainWindow->pos();
             *mpPointMoveStart = event->globalPos();
             mpMainWindow->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
         }
     }
     event->accept();
    QToolBar::mouseMoveEvent(event);
}

bool FramelessWindowToolBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange: //标题修改
        mpLabelTitle->setText(mpMainWindow->windowTitle());
        break;
    case QEvent::WindowIconChange: //图标修改
        mpLabelIcon->setFixedSize(mpMainWindow->iconSize());
        mpLabelIcon->setScaledContents(true);
        mpLabelIcon->setPixmap(mpMainWindow->windowIcon().pixmap(mpLabelIcon->size()));
        break;
    case QEvent::WindowStateChange:
        UpdateMaximizeButton();
        break;
    default:
        break;
    }
    return QToolBar::eventFilter(obj, event);
}

void FramelessWindowToolBar::UpdateMaximizeButton()
{
    if (mpMainWindow == Q_NULLPTR) return;

    if (mpMainWindow->isTopLevel())
    {
        if (mpMainWindow->isMaximized())
        {
            mpToolBtnMax->setToolTip("Restore");
            mpToolBtnMax->setIcon(style()->standardPixmap(QStyle::SP_TitleBarNormalButton));
        }
        else
        {
            mpToolBtnMax->setToolTip("Maximize");
            mpToolBtnMax->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
        }
    }
}

bool FramelessWindowToolBar::IsCursorOnEdge(QPoint *pMousePos)
{
    bool retValue = false;
    if (pMousePos == Q_NULLPTR) return retValue;

    int globalMouseX = pMousePos->x();
    int globalMouseY = pMousePos->y();
    QRect frameRect = this->frameGeometry();
    int frameLeft = frameRect.left();
    int frameRight = frameRect.right();
    int frameTop = frameRect.top();
    int frameBottom = frameRect.bottom();

    bool bOnEdgeLeft = globalMouseX >= frameLeft && globalMouseX <= frameLeft + mnBorderSize;
    bool bOnEdgeRight = globalMouseX <= frameRight && globalMouseX >= frameRight - mnBorderSize;
    bool bOnEdgeTop = globalMouseY >= frameTop && globalMouseY <= frameTop + mnBorderSize;
    bool bOnEdgeBottom = globalMouseY <= frameBottom && globalMouseY >= frameBottom - mnBorderSize;
    retValue = bOnEdgeLeft || bOnEdgeRight || bOnEdgeTop || bOnEdgeBottom;

    return retValue;
}

void FramelessWindowToolBar::SlotMaximizeButtonClicked()
{
    if (mpMainWindow == Q_NULLPTR) return;

    if (mpMainWindow->isMaximized())
    {
        mpMainWindow->showNormal();
    }
    else
    {
        *mpRectNormalWindow = mpMainWindow->rect();
        mpMainWindow->showMaximized();
    }
}
