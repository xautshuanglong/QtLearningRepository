#pragma once

#include <QWidget>
#include <QString>

class QRubberBand;
class QToolButton;
class QResizeEvent;
class QMouseEvent;
class QHoverEvent;
class QWindowStateChangeEvent;

class TitleBarWidget : public QWidget
{
    Q_OBJECT
public:
    TitleBarWidget(QWidget *parent = Q_NULLPTR);
    ~TitleBarWidget();

private:
    void InitUI();
    void HandleEventResize(QObject *obj, QResizeEvent *event);
    void HandleEventMousePress(QObject *obj, QMouseEvent *event);
    void HandleEventMouseRelease(QObject *obj, QMouseEvent *event);
    void HandleEventMouseMove(QObject *obj, QMouseEvent *event);
    void HandleEventMouseLeave(QObject *obj, QMouseEvent *event);
    void HandleEventHoverMove(QObject *obj, QHoverEvent *event);
    void HandleEventWindowStateChange(QObject *obj, QWindowStateChangeEvent *event);
    void CheckCursorPosition(const QPoint& globalMousePos, const QRect& frameRect);
    void UpdateCursorShape(const QPoint& globalMousePos);
    void ResizeWindow(const QPoint& globalMousePos);
    void UpdateMaximizeButton();

private slots:
    void SlotMaximizeButtonClicked();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QWidget        *mpWidgetParent;
    QRubberBand    *mpRubberBand;
    QToolButton    *mpBtnMinimum;
    QToolButton    *mpBtnMaximum;
    QToolButton    *mpBtnClose;
    QRect          *mpRectNormalWindow;
    QPoint         *mpPointMoveStart;
    int             mnBorderWidth;
    bool            mbLeftDoublePressed = false;
    bool            mbLeftBtnPressedTitle = false;
    bool            mbLeftBtnPressedParent = false;
    bool            mbCursorShapeChanged = false;
    bool            mbOnCornerTopLeft = false;
    bool            mbOnCornerTopRight = false;
    bool            mbOnCornerBottomLeft = false;
    bool            mbOnCornerBottomRight = false;
    bool            mbOnEdge = false;
    bool            mbOnEdgeLeft = false;
    bool            mbOnEdgeRight = false;
    bool            mbOnEdgeTop = false;
    bool            mbOnEdgeBottom = false;
};
