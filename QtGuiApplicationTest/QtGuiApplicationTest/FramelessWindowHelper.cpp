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

    bool         m_bMouseLeftPressed = false;       // ����������״̬��¼
    bool         m_bMouseLeftDoublePressed = false; // ������˫��״̬��¼
    int          m_nBorderSize = 5;                 // ���ű߿���
    QRect        mRectNormalWin;                    // ���ǰ�Ĵ��ھ��Σ����ڻָ����ڲ���������ƫ����
    QPoint       mPointStart;                       // �����ƶ�ʱ��ʼλ�ü�¼
    QWidget     *mpWidgetParent;                    // ��ȡ������ָ�룬Ĭ��ΪQMainWindow
    QLabel      *mpLableIcon;                       // ����ͼ��
    QLabel      *mpLabelTitle;                      // ���ڱ���
    QWidget     *mpWidgetToolBarSeat;               // �����в�ռλ
    QToolButton *mpToolButtonMini;                  // ��С��
    QToolButton *mpToolButtonMax;                   // ���
    QToolButton *mpToolButtonClose;                 // �ر�
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
