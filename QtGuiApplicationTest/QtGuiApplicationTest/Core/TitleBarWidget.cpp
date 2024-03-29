#include "TitleBarWidget.h"

#include <QEvent>
#include <QResizeEvent>
#include <QLabel>
#include <QToolButton>
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QRubberBand>
#include <QWindowStateChangeEvent>

#include "JCB_Logger/LogUtil.h"

TitleBarWidget::TitleBarWidget(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
    , mnBorderWidth(3)
{
    mpWidgetParent = parent;
    mpRubberBand = new QRubberBand(QRubberBand::Rectangle);
    mpRectNormalWindow = new QRect();
    mpPointMoveStart = new QPoint();

    this->InitUI();

    if (mpWidgetParent != nullptr)
    {
        mpWidgetParent->installEventFilter(this);
        mpWidgetParent->setAttribute(Qt::WA_Hover);
    }
}

TitleBarWidget::~TitleBarWidget()
{
    delete mpRubberBand;
    delete mpRectNormalWindow;
    delete mpPointMoveStart;
}

void TitleBarWidget::InitUI()
{
    QLabel *pLabelIcon = new QLabel(this);
    pLabelIcon->setFixedWidth(80);
    QLabel *pLabelTitle = new QLabel(this);
    //pLabelTitle->setStyleSheet("background-color: yellow;");
    pLabelTitle->setAlignment(Qt::AlignCenter);
    pLabelTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mpBtnMinimum = new QToolButton(this);
    mpBtnMaximum = new QToolButton(this);
    mpBtnClose = new QToolButton(this);

    mpBtnMinimum->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarMinButton));
    mpBtnMaximum->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
    mpBtnClose->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarCloseButton));

    QHBoxLayout *pHorizontalLayout = new QHBoxLayout(this);
    pHorizontalLayout->setSpacing(3);
    pHorizontalLayout->setMargin(0);
    pHorizontalLayout->setContentsMargins(0, 0, 5, 0);

    pHorizontalLayout->addWidget(pLabelIcon);
    pHorizontalLayout->addWidget(pLabelTitle);
    pHorizontalLayout->addWidget(mpBtnMinimum);
    pHorizontalLayout->addWidget(mpBtnMaximum);
    pHorizontalLayout->addWidget(mpBtnClose);

    if (mpWidgetParent != Q_NULLPTR)
    {
        QIcon winIcon = mpWidgetParent->windowIcon();
        mpWidgetParent->setWindowFlags(mpWidgetParent->windowFlags() | Qt::FramelessWindowHint);

        //pLabelIcon->setPixmap(winIcon.pixmap(QSize(24, 24)));
        pLabelTitle->setText(mpWidgetParent->windowTitle());
        this->connect(mpBtnMaximum, SIGNAL(clicked()), this, SLOT(SlotMaximizeButtonClicked()));
        this->connect(mpBtnMinimum, SIGNAL(clicked()), mpWidgetParent, SLOT(showMinimized()));
        this->connect(mpBtnClose, SIGNAL(clicked()), mpWidgetParent, SLOT(close()));
    }
}

void TitleBarWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        this->SlotMaximizeButtonClicked();
        mbLeftDoublePressed = true;
    }
    event->accept();
    //QWidget::mouseDoubleClickEvent(event);
}

void TitleBarWidget::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button() && !mbLeftDoublePressed)
    {
        mbLeftBtnPressedTitle = true;
        *mpPointMoveStart = event->globalPos();
    }
    event->accept();
    //QWidget::mousePressEvent(event);
}

void TitleBarWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        mbLeftBtnPressedTitle = false;
        mbLeftDoublePressed = false;
    }
    event->accept();
}

void TitleBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mbLeftBtnPressedTitle)
    {
        if (mpWidgetParent->isMaximized())
        {
            QPoint curMousePos = event->globalPos();
            QPoint maxWindowPos = mpWidgetParent->pos();
            QSize maxWinSize = mpWidgetParent->size();
            float xPercent = (curMousePos.x() - maxWindowPos.x()) * 1.0f / maxWinSize.width();
            QPoint newNormalPos;
            newNormalPos.setX(mpRectNormalWindow->left() + mpRectNormalWindow->width() * xPercent);
            newNormalPos.setY(mpRectNormalWindow->top() + curMousePos.y() - maxWindowPos.y());
            QPoint movePoint = curMousePos - newNormalPos;
            *mpPointMoveStart = curMousePos;

            mpWidgetParent->showNormal(); // 当为最大化时，还原为正常模式
            mpRectNormalWindow->moveTopLeft(movePoint);
            mpWidgetParent->setGeometry(*mpRectNormalWindow);
        }
        else
        {
            QPoint movePoint = event->globalPos() - *mpPointMoveStart;
            QPoint widgetPos = mpWidgetParent->pos();
            *mpPointMoveStart = event->globalPos();
            mpWidgetParent->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
        }
    }
    event->accept();
}


bool TitleBarWidget::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Resize:
        this->HandleEventResize(obj, static_cast<QResizeEvent*>(event));
        break;
    case QEvent::MouseButtonPress:
        this->HandleEventMousePress(obj, static_cast<QMouseEvent*>(event));
        break;
    case QEvent::MouseButtonRelease:
        this->HandleEventMouseRelease(obj, static_cast<QMouseEvent*>(event));
        break;
    case QEvent::MouseMove:
        this->HandleEventMouseMove(obj, static_cast<QMouseEvent*>(event));
        break;
    case QEvent::HoverMove:
        this->HandleEventHoverMove(obj, static_cast<QHoverEvent*>(event));
        break;
    case QEvent::WindowStateChange:
        this->HandleEventWindowStateChange(obj, static_cast<QWindowStateChangeEvent*>(event));
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}

void TitleBarWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TitleBarWidget::HandleEventResize(QObject *obj, QResizeEvent *event)
{
    QRect titleBarRect = this->geometry();
    titleBarRect.setWidth(event->size().width());
    this->setGeometry(titleBarRect);
}

void TitleBarWidget::HandleEventMousePress(QObject *obj, QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftBtnPressedParent = true;

        QRect frameRect = mpWidgetParent->frameGeometry();
        QPoint globalMousePos = event->globalPos();
        UpdateCursorShape(globalMousePos);

        if (mbOnEdge)
        {
            mpRubberBand->setGeometry(frameRect);
            mpRubberBand->show();
        }
    }
}

void TitleBarWidget::HandleEventMouseRelease(QObject *obj, QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftBtnPressedParent = false;
        if (mpRubberBand && mpRubberBand->isVisible())
        {
            mpRubberBand->hide();
            mpWidgetParent->setGeometry(mpRubberBand->geometry());
        }
        QPoint globalPos = event->globalPos();
        UpdateCursorShape(globalPos);
    }
}

void TitleBarWidget::HandleEventMouseMove(QObject *obj, QMouseEvent *event)
{
    if (mbLeftBtnPressedParent)
    {
        if (mbOnEdge)
        {
            QPoint globalMousePos = event->globalPos();
            this->ResizeWindow(globalMousePos);
        }
    }
}

void TitleBarWidget::HandleEventMouseLeave(QObject *obj, QMouseEvent *event)
{
    int  i = 0;
}

void TitleBarWidget::HandleEventHoverMove(QObject *obj, QHoverEvent *event)
{
    if (!mbLeftBtnPressedParent)
    {
        QPoint globalMousePos = mpWidgetParent->mapToGlobal(event->pos());
        UpdateCursorShape(globalMousePos);
    }
}

void TitleBarWidget::HandleEventWindowStateChange(QObject *obj, QWindowStateChangeEvent *event)
{
    this->UpdateMaximizeButton();
}

void TitleBarWidget::CheckCursorPosition(const QPoint& globalMousePos, const QRect& frameRect)
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

void TitleBarWidget::UpdateCursorShape(const QPoint& globalMousePos)
{
    if (mpWidgetParent == Q_NULLPTR) return;

    if (mpWidgetParent->isFullScreen() || mpWidgetParent->isMaximized())
    {
        if (mbCursorShapeChanged)
        {
            mpWidgetParent->unsetCursor();
        }
    }

    QRect mainWindowRect = mpWidgetParent->frameGeometry();
    this->CheckCursorPosition(globalMousePos, mainWindowRect);

    if (mbOnCornerTopLeft || mbOnCornerBottomRight)
    {
        mpWidgetParent->setCursor(Qt::SizeFDiagCursor);
        mbCursorShapeChanged = true;
    }
    else if (mbOnCornerBottomLeft || mbOnCornerTopRight)
    {
        mpWidgetParent->setCursor(Qt::SizeBDiagCursor);
        mbCursorShapeChanged = true;
    }
    else if (mbOnEdgeLeft || mbOnEdgeRight)
    {
        mpWidgetParent->setCursor(Qt::SizeHorCursor);
        mbCursorShapeChanged = true;
    }
    else if (mbOnEdgeTop || mbOnEdgeBottom)
    {
        mpWidgetParent->setCursor(Qt::SizeVerCursor);
        mbCursorShapeChanged = true;
    }
    else if (mbCursorShapeChanged)
    {
        mpWidgetParent->unsetCursor();
        mbCursorShapeChanged = false;
    }
}

void TitleBarWidget::ResizeWindow(const QPoint& globalMousePos)
{
    QRect originalWinRect;
    originalWinRect = mpRubberBand->frameGeometry();

    int left = 0, top = 0, right = 0, bottom = 0;
    originalWinRect.getCoords(&left, &top, &right, &bottom);

    int minWidth = mpWidgetParent->minimumWidth();
    int minHeight = mpWidgetParent->minimumHeight();

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

        mpRubberBand->setGeometry(newRect);
    }
}

void TitleBarWidget::SlotMaximizeButtonClicked()
{
    if (mpWidgetParent == Q_NULLPTR) return;

    if (mpWidgetParent->isMaximized())
    {
        mpWidgetParent->showNormal();
    }
    else
    {
        *mpRectNormalWindow = mpWidgetParent->rect();
        mpWidgetParent->showMaximized();
    }
}

void TitleBarWidget::UpdateMaximizeButton()
{
    if (mpWidgetParent == Q_NULLPTR) return;

    if (mpWidgetParent->isTopLevel())
    {
        if (mpWidgetParent->isMaximized())
        {
            mpBtnMaximum->setIcon(style()->standardPixmap(QStyle::SP_TitleBarNormalButton));
        }
        else
        {
            mpBtnMaximum->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
        }
    }
}
