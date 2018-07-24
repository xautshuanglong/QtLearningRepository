#include "TitleBar.h"

#include <QApplication>
#include <QToolButton>
#include <QMainWindow>
#include <QMouseEvent>
#include <QEvent>
#include <QStyle>
#include <QPoint>
#include <QLabel>
#include <QIcon>

class TitleBarPrivate
{
public:
    bool mouse_left_pressing = false;    // 鼠标左键按下状态记录
    bool mouse_left_doublePress = false; // 鼠标左键双击状态记录
    int borderSize;                      // 缩放边框检测
    QRect normalWinRect;                 // 最大化前的窗口矩形，用于恢复窗口并计算坐标偏移量
    QPoint move_start_point;             // 窗口移动时起始位置记录
    QMainWindow *w;                      // 获取主窗口指针，默认为QMainWindow
    QLabel *icon_label;                  // 窗口图标
    QLabel *title_label;                 // 窗口标题
    QWidget* toolBar_seat;               // 标题中部占位
    QToolButton *toolButton_mini;        // 最小化
    QToolButton *toolButton_max;         // 最大化
    QToolButton *toolButton_close;       // 关闭
};

TitleBar::TitleBar(QWidget *parent) : QToolBar(parent)
{
    m_private = new TitleBarPrivate();
    m_private->borderSize = 5;
    m_private->w = (QMainWindow*)window();
    //自定义标题初始化--直接操作主窗口，降低使用复杂性
    Qt::WindowFlags oldFlags = m_private->w->windowFlags();
    m_private->w->setWindowFlags(oldFlags | Qt::FramelessWindowHint); //主窗口隐藏标题栏
    m_private->w->addToolBar(this);//把标题栏添加到主窗口
    m_private->w->installEventFilter(this);//事件过滤器
    //设置此标题位置
    setMovable(false);//禁止拖动
    //左侧显示
    m_private->icon_label = new QLabel(this);
    m_private->title_label = new QLabel(this);
    m_private->icon_label->setFixedSize(m_private->w->iconSize());
    m_private->icon_label->setScaledContents(true);
    m_private->icon_label->setPixmap(m_private->w->windowIcon().
        pixmap(m_private->icon_label->size()));
    m_private->title_label->setText(m_private->w->windowTitle());
    addWidget(m_private->icon_label);
    addWidget(m_private->title_label);
    //中部占位
    m_private->toolBar_seat = new QWidget();
    m_private->toolBar_seat->setSizePolicy(QSizePolicy::Expanding,
        QSizePolicy::Expanding);//长宽自动扩展
    addWidget(m_private->toolBar_seat);
    //右侧显示
    m_private->toolButton_mini = new QToolButton(this);
    m_private->toolButton_max = new QToolButton(this);
    m_private->toolButton_close = new QToolButton(this);

    m_private->toolButton_mini->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMinButton));
    m_private->toolButton_max->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
    m_private->toolButton_close->setIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton));

    m_private->toolButton_mini->setToolTip("Minimize");
    m_private->toolButton_max->setToolTip("Maximize");
    m_private->toolButton_close->setToolTip("Close");

    //m_private->toolButton_mini->setToolTip(QString::fromLocal8Bit("最小化"));
    //m_private->toolButton_max->setToolTip(QStringLiteral("最大化"));
    //m_private->toolButton_close->setToolTip(QStringLiteral("关闭"));

    addWidget(m_private->toolButton_mini);
    addWidget(m_private->toolButton_max);
    addWidget(m_private->toolButton_close);

    connect(m_private->toolButton_mini, SIGNAL(clicked(bool)), m_private->w, SLOT(showMinimized()));
    connect(m_private->toolButton_max, SIGNAL(clicked(bool)), this, SLOT(SlotMaximizeButtonClicked()));
    connect(m_private->toolButton_close, SIGNAL(clicked(bool)), m_private->w, SLOT(close()));
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        SlotMaximizeButtonClicked();
        m_private->mouse_left_doublePress = true;
    }
    event->accept();
    //QToolBar::mouseDoubleClickEvent(event);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button() && !m_private->mouse_left_doublePress)
    {
        m_private->mouse_left_pressing = true;
        m_private->move_start_point = event->globalPos();
    }
    event->accept();
    //QToolBar::mousePressEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        m_private->mouse_left_pressing = false;
        m_private->mouse_left_doublePress = false;
    }
    event->accept();
    //QToolBar::mouseReleaseEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    auto w = window();//获取主窗口指针
    if (m_private->mouse_left_pressing)
    {
        if (w->isMaximized())
        {
            QPoint curMousePos = event->globalPos();
            QPoint maxWindowPos = w->pos();
            QSize maxWinSize = w->size();
            float xPercent = (curMousePos.x() - maxWindowPos.x()) * 1.0f / maxWinSize.width();
            QPoint newNormalPos;
            newNormalPos.setX(m_private->normalWinRect.left() + m_private->normalWinRect.width() * xPercent);
            newNormalPos.setY(m_private->normalWinRect.top() + curMousePos.y() - maxWindowPos.y());
            QPoint movePoint = curMousePos - newNormalPos;
            m_private->move_start_point = curMousePos;

            w->showNormal(); // 当为最大化时，还原为正常模式
            m_private->normalWinRect.moveTopLeft(movePoint);
            w->setGeometry(m_private->normalWinRect);
        }
        else
        {
            QPoint movePoint = event->globalPos() - m_private->move_start_point;
            QPoint widgetPos = w->pos();
            m_private->move_start_point = event->globalPos();
            w->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
        }
    }
    event->accept();
    //QToolBar::mouseMoveEvent(event);
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange: //标题修改
        m_private->title_label->setText(m_private->w->windowTitle());
        break;
    case QEvent::WindowIconChange: //图标修改
        m_private->icon_label->setFixedSize(m_private->w->iconSize());
        m_private->icon_label->setScaledContents(true);
        m_private->icon_label->setPixmap(m_private->w->windowIcon().pixmap(m_private->icon_label->size()));
        break;
    case QEvent::WindowStateChange:
        UpdateMaximizeButton();
        break;
    default:
        break;
    }

    return QToolBar::eventFilter(obj, event);
}

void TitleBar::SlotMaximizeButtonClicked()
{
    QWidget *pMainWindow = window();
    if (pMainWindow->isMaximized())
    {
        pMainWindow->showNormal();
    }
    else
    {
        m_private->normalWinRect = pMainWindow->rect();
        pMainWindow->showMaximized();
    }
}

void TitleBar::UpdateMaximizeButton()
{
    QWidget *pMainWindow = this->window();
    if (pMainWindow->isTopLevel())
    {
        if (pMainWindow->isMaximized())
        {
            m_private->toolButton_max->setToolTip("Restore");
            m_private->toolButton_max->setIcon(style()->standardPixmap(QStyle::SP_TitleBarNormalButton));
        }
        else
        {
            m_private->toolButton_max->setToolTip("Maximize");
            m_private->toolButton_max->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
        }
    }
}
