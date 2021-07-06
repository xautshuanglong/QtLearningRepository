#include "SuspendedScrollBar.h"

// QT Headers
#include <QEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QStyleOption>

// Self Headers
#include <LogUtil.h>

SuspendedScrollBar::SuspendedScrollBar(QScrollBar *pShadowScrollBar, QWidget *parent /* = Q_NULLPTR */)
    : QScrollBar(parent)
    , mpScrollBarSibling(Q_NULLPTR)
{
    mpScrollBarShadow = pShadowScrollBar;
    mpScrollBarParent = parent;
    if (mpScrollBarShadow->orientation() == Qt::Vertical)
    {
        mCustomHeight = 100;
        mCustomWidth = 20;
    }
    else
    {
        mCustomHeight = 20;
        mCustomWidth = 100;
    }
    this->hide();

    if (parent != Q_NULLPTR)
    {
        parent->installEventFilter(this);
    }

    if (mpScrollBarShadow != Q_NULLPTR)
    {
        this->setOrientation(mpScrollBarShadow->orientation());
        this->setValue(mpScrollBarShadow->value());
        this->setRange(mpScrollBarShadow->minimum(), mpScrollBarShadow->maximum());
        this->setPageStep(mpScrollBarShadow->pageStep());

        this->connect(this, SIGNAL(valueChanged(int)), mpScrollBarShadow, SLOT(setValue(int)));

        //this->connect(mpScrollBarShadow, SIGNAL(valueChanged(int)), SLOT(setValue(int)));
        //this->connect(mpScrollBarShadow, SIGNAL(rangeChanged(int, int)), SLOT(setRange(int, int)));

        this->connect(mpScrollBarShadow, SIGNAL(valueChanged(int)), SLOT(SlotScrollBarValueChange(int)));
        this->connect(mpScrollBarShadow, SIGNAL(rangeChanged(int, int)), SLOT(SlotScrollBarRangeChanged(int, int)));
    }
}

SuspendedScrollBar::~SuspendedScrollBar()
{
}

void SuspendedScrollBar::HandleEventResize(QObject *obj, QResizeEvent *event)
{
    mScrollBarSize = event->size();
    QRect scrollBarRect(0, 0, 0, 0);
    if (mpScrollBarShadow->orientation() == Qt::Vertical)
    {
        scrollBarRect.setHeight(mScrollBarSize.height());
        scrollBarRect.setLeft(mScrollBarSize.width() - mCustomWidth);
        scrollBarRect.setWidth(mCustomWidth);
    }
    else
    {
        scrollBarRect.setTop(mScrollBarSize.height() - mCustomHeight);
        scrollBarRect.setHeight(mCustomHeight);
        scrollBarRect.setWidth(mScrollBarSize.width());
    }
    this->setGeometry(scrollBarRect);
}

void SuspendedScrollBar::HandleMouseEnter(QObject *obj, QMouseEvent *event)
{
    if (this->maximum() > 0)
    {
        this->show();
    }
}

void SuspendedScrollBar::HandleMouseLeave(QObject *obj, QMouseEvent *event)
{
    this->hide();
}

// 垂直滚动条的 sibling 代表水平滚动条；水平滚动条的 sibling 代表垂直滚动条。作用：同时存在 垂直 和 水平 滚动条时，防止重叠
void SuspendedScrollBar::ResizeSibling()
{
    if (mpScrollBarSibling == Q_NULLPTR) return;

    QRect scrollBarRect(0, 0, 0, 0);
    if (mpScrollBarSibling->orientation() == Qt::Vertical)
    {
        QRect siblingRect = mpScrollBarSibling->geometry();
        scrollBarRect.setLeft(siblingRect.left());
        scrollBarRect.setTop(siblingRect.top());
        scrollBarRect.setWidth(siblingRect.width());
        scrollBarRect.setHeight(siblingRect.height() - mCustomHeight);
    }
    else
    {
        QRect siblingRect = mpScrollBarSibling->geometry();
        scrollBarRect.setLeft(siblingRect.left());
        scrollBarRect.setTop(siblingRect.top());
        scrollBarRect.setHeight(siblingRect.height());
        scrollBarRect.setWidth(siblingRect.width() - mCustomWidth);
    }
    mpScrollBarSibling->setGeometry(scrollBarRect);
}

void SuspendedScrollBar::paintEvent(QPaintEvent *event)
{
    //QStyleOptionSlider opt;
    //opt.init(this);
    //QPainter p(this);
    //this->style()->drawComplexControl(QStyle::CC_ScrollBar, &opt, &p, this);

    QScrollBar::paintEvent(event);
}

bool SuspendedScrollBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Resize:
        this->HandleEventResize(obj, static_cast<QResizeEvent*>(event));
        break;
    case QEvent::Enter:
        this->HandleMouseEnter(obj, static_cast<QMouseEvent*>(event));
        break;
    case QEvent::Leave:
        this->HandleMouseLeave(obj, static_cast<QMouseEvent*>(event));
        break;
    default:
        break;
    }
    return QScrollBar::eventFilter(obj, event);
}

void SuspendedScrollBar::SlotScrollBarValueChange(int value)
{
    //LogUtil::Debug(CODE_LOCATION, "ScrollBar value = %d", value);
    this->setValue(value);
}

void SuspendedScrollBar::SlotScrollBarRangeChanged(int minValue, int maxValue)
{
    //LogUtil::Debug(CODE_LOCATION, "ScrollBar range : %d - %d", minValue, maxValue);

    //this->setMinimum(minValue);
    //this->setMaximum(maxValue);

    this->setRange(minValue, maxValue);
    this->setPageStep(0.5 * (this->height() + maxValue));

    if (maxValue > 0)
    {
        this->ResizeSibling();
    }
}
