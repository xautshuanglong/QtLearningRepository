#include "TitleBarWidget.h"

#include <QEvent>
#include <QResizeEvent>
#include <QLabel>
#include <QToolButton>
#include <QStyle>
#include <QHBoxLayout>
#include <QRubberBand>
#include <QWindowStateChangeEvent>

#include <LogUtil.h>

TitleBarWidget::TitleBarWidget(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
    , mnBorderWidth(5)
{
    mpWidgetParent = parent;
    mpRubberBand = new QRubberBand(QRubberBand::Rectangle);
    mpRectNormalWindow = new QRect();

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
}

void TitleBarWidget::InitUI()
{
    this->setStyleSheet(""
                        ".QWidget"
                        "{"
                        "   border: 1px solid green;"
                        "}"
    );

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
    pHorizontalLayout->setSpacing(0);
    pHorizontalLayout->setMargin(3);

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
        mbLeftBtnPressed = true;

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
        mbLeftBtnPressed = false;
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
    if (mbLeftBtnPressed)
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
    if (!mbLeftBtnPressed)
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
    if (mbRubberBandOnResize)
    {
        originalWinRect = mpRubberBand->frameGeometry();
    }
    else
    {
        originalWinRect = mpWidgetParent->frameGeometry();
    }

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

        if (mbRubberBandOnResize)
        {
            mpRubberBand->setGeometry(newRect);
        }
        else
        {
            mpWidgetParent->setGeometry(newRect);
        }
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
