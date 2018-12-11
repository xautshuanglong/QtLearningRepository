#include "SuspendedScrollBar.h"

// QT Headers
#include <QEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QStyleOption>

SuspendedScrollBar::SuspendedScrollBar(QScrollBar *pShadowScrollBar, QWidget *parent /* = Q_NULLPTR */)
    : QScrollBar(parent)
{
    mpScrollBarShadow = pShadowScrollBar;

    this->hide();
    this->setOrientation(pShadowScrollBar->orientation());
    this->setRange(0, 100);
    this->setValue(0);
    if (parent != Q_NULLPTR)
    {
        parent->installEventFilter(this);
    }

    //this->connect(mpScrollBarShadow, SIGNAL(valueChanged(int)), SLOT(setValue(int)));
    //this->connect(mpScrollBarShadow, SIGNAL(rangeChanged(int, int)), SLOT(setRange(int, int)));

    this->connect(mpScrollBarShadow, SIGNAL(valueChanged(int)), SLOT(SlotScrollBarValueChange(int)));
    this->connect(mpScrollBarShadow, SIGNAL(rangeChanged(int, int)), SLOT(SlotScrollBarRangeChanged(int, int)));
}

//SuspendedScrollBar::SuspendedScrollBar(Qt::Orientation orientation, QWidget *parent /* = Q_NULLPTR */)
//    : QScrollBar(orientation, parent)
//{
//    this->InitUI();
//}

SuspendedScrollBar::~SuspendedScrollBar()
{
}

void SuspendedScrollBar::HandleEventResize(QObject *obj, QResizeEvent *event)
{
    //QRect scrollBarRect = this->geometry();
    //scrollBarRect.moveTo(event->size().width()-scrollBarRect.width(), 0);
    //scrollBarRect.setHeight(event->size().height());
    //this->setGeometry(scrollBarRect);

    //this->setGeometry(mpScrollBarShadow->geometry());
}

void SuspendedScrollBar::HandleMouseEnter(QObject *obj, QMouseEvent *event)
{
    this->show();
}

void SuspendedScrollBar::HandleMouseLeave(QObject *obj, QMouseEvent *event)
{
    this->hide();
}

void SuspendedScrollBar::paintEvent(QPaintEvent *event)
{
    QStyleOptionSlider opt;
    opt.init(this);
    QPainter p(this);
    this->style()->drawComplexControl(QStyle::CC_ScrollBar, &opt, &p, this);
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
    this->setValue(value);
}

void SuspendedScrollBar::SlotScrollBarRangeChanged(int minValue, int maxValue)
{
    this->setMinimum(minValue);
    this->setMaximum(maxValue);
}
