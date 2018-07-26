#include "FramelessWindowHelper.h"

#include <QEvent>
#include <QWidget>
#include <QLabel>
#include <QToolButton>

class FramelessWindowHelperPrivate
{
public:
    bool m_bWidgetMovable = true;
    bool m_bWidgetResizable = true;
    bool m_bRubberBandOnResize = true;
    bool m_bRubberBandOnMove = true;

    bool         m_bMouseLeftPressed = false;       // 鼠标左键按下状态记录
    bool         m_bMouseLeftDoublePressed = false; // 鼠标左键双击状态记录
    int          m_nBorderSize = 5;                 // 缩放边框检测
    QRect        mRectNormalWin;                    // 最大化前的窗口矩形，用于恢复窗口并计算坐标偏移量
    QPoint       mPointStart;                       // 窗口移动时起始位置记录
    QWidget     *mpWidgetParent;                    // 获取主窗口指针，默认为QMainWindow
    QLabel      *mpLableIcon;                       // 窗口图标
    QLabel      *mpLabelTitle;                      // 窗口标题
    QWidget     *mpWidgetToolBarSeat;               // 标题中部占位
    QToolButton *mpToolButtonMini;                  // 最小化
    QToolButton *mpToolButtonMax;                   // 最大化
    QToolButton *mpToolButtonClose;                 // 关闭
};

FramelessWindowHelper::FramelessWindowHelper(QWidget* parent)
    : QObject(parent)
{
    parent->installEventFilter(this);
}

FramelessWindowHelper::~FramelessWindowHelper()
{
}

bool FramelessWindowHelper::eventFilter(QObject *obj, QEvent *event)
{
    int i = 0;
    switch (event->type())
    {
    case QEvent::MouseMove:
        return HandleEventMouseMove(obj, event);
    case QEvent::HoverMove:
        return HandleEventHoverMove(obj, event);
    case QEvent::MouseButtonPress:
        return HandleEventMouseButtonPress(obj, event);
    case QEvent::MouseButtonRelease:
        return HandleEventMouseButtonRelease(obj, event);
    case QEvent::Leave:
        return HandleEventMouseLeave(obj, event);
    case QEvent::WindowTitleChange:
        return HandleEventWindowTitleChange(obj, event);
    case QEvent::WindowIconChange:
        return HandleEventWindowIconChange(obj, event);
    case QEvent::WindowStateChange:
        return HandleEventWindowStateChange(obj, event);
    default:
        return QObject::eventFilter(obj, event);
    }

    //return QObject::eventFilter(obj, event);
}

bool FramelessWindowHelper::HandleEventMouseMove(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWindowHelper::HandleEventHoverMove(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWindowHelper::HandleEventMouseButtonPress(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWindowHelper::HandleEventMouseButtonRelease(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}

bool FramelessWindowHelper::HandleEventMouseLeave(QObject *obj, QEvent *event)
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

bool FramelessWindowHelper::HandleEventWindowStateChange(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}
