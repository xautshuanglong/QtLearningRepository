#ifndef FRAMELESS_HELPER_H
#define FRAMELESS_HELPER_H

#include <QObject>
#include <QPoint>

class QWidget;
class QEvent;
class QMouseEvent;
class QHoverEvent;
class QRubberBand;
class FramelessHelperPrivate;

class FramelessHelper : public QObject
{
    Q_OBJECT

public:
    explicit FramelessHelper(QObject *parent = 0);
    ~FramelessHelper();
    void activateOn(QWidget *topLevelWidget);
    void removeFrom(QWidget *topLevelWidget);
    void setWidgetMovable(bool movable);
    void setWidgetResizable(bool resizable);
    void setRubberBandOnMove(bool movable);
    void setRubberBandOnResize(bool resizable);
    void setBorderWidth(uint width);
    void setTitleHeight(uint height);
    bool widgetResizable();
    bool widgetMovable();
    bool rubberBandOnMove();
    bool rubberBandOnResisze();
    uint borderWidth();
    uint titleHeight();

protected:
    // �¼����ˣ������ƶ������ŵ�
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    FramelessHelperPrivate *d;
};


/*****
* CursorPosCalculator
* ��������Ƿ�λ�����ϡ��ҡ��¡����Ͻǡ����½ǡ����Ͻǡ����½�
*****/
class CursorPosCalculator
{
public:
    explicit CursorPosCalculator();
    void reset();
    void recalculate(const QPoint &globalMousePos, const QRect &frameRect);

public:
    bool m_bOnEdges : true;
    bool m_bOnLeftEdge : true;
    bool m_bOnRightEdge : true;
    bool m_bOnTopEdge : true;
    bool m_bOnBottomEdge : true;
    bool m_bOnTopLeftEdge : true;
    bool m_bOnBottomLeftEdge : true;
    bool m_bOnTopRightEdge : true;
    bool m_bOnBottomRightEdge : true;

    static int m_nBorderWidth;
    static int m_nTitleHeight;
};


/*****
* WidgetData
* ���������ʽ���ƶ����塢���Ŵ���
*****/

class WidgetData
{
public:
    explicit WidgetData(FramelessHelperPrivate *d, QWidget *pTopLevelWidget);
    ~WidgetData();
    QWidget* widget();
    // ��������¼�-���������¡��ͷš��ƶ�
    void handleWidgetEvent(QEvent *event);
    // ������Ƥ��״̬
    void updateRubberBandStatus();

private:
    // ���������ʽ
    void updateCursorShape(const QPoint &gMousePos);
    // ���ô����С
    void resizeWidget(const QPoint &gMousePos);
    // �ƶ�����
    void moveWidget(const QPoint &gMousePos);
    // ������갴��
    void handleMousePressEvent(QMouseEvent *event);
    // ��������ͷ�
    void handleMouseReleaseEvent(QMouseEvent *event);
    // ��������ƶ�
    void handleMouseMoveEvent(QMouseEvent *event);
    // ��������뿪
    void handleLeaveEvent(QEvent *event);
    // ����������
    void handleHoverMoveEvent(QHoverEvent *event);

private:
    FramelessHelperPrivate *d;
    QRubberBand *m_pRubberBand;
    QWidget *m_pWidget;
    QPoint m_ptDragPos;
    CursorPosCalculator m_pressedMousePos;
    CursorPosCalculator m_moveMousePos;
    bool m_bLeftButtonPressed;
    bool m_bCursorShapeChanged;
    bool m_bLeftButtonTitlePressed;
    Qt::WindowFlags m_windowFlags;
};


#endif //FRAMELESS_HELPER_H
