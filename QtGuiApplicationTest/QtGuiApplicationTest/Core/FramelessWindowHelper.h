#pragma once

#include <QObject>
#include <QToolBar>
#include <QRect>
#include <QPoint>

class QRect;
class QPoint;
class QToolButton;
class QLabel;
class QWidget;
class QMainWindow;
class QRubberBand;
class QMouseEvent;
class QWindowStateChangeEvent;

class FramelessWindowToolBar : public QToolBar
{
    Q_OBJECT

private:
    bool         mbLeftPressed = false;
    bool         mbLeftDoublePressed = false;
    bool         mbCursorOnEdge = false;
    int          mnBorderSize;
    QRect       *mpRectNormalWindow;
    QPoint      *mpPointMoveStart;
    QToolButton *mpToolBtnMax;
    QToolButton *mpToolBtnMin;
    QToolButton *mpToolBtnClose;
    QLabel      *mpLabelIcon;
    QLabel      *mpLabelTitle;
    QWidget     *mpToolBarSeat;
    QMainWindow *mpMainWindow = Q_NULLPTR;

public:
    explicit FramelessWindowToolBar(QWidget *parent = Q_NULLPTR);
    ~FramelessWindowToolBar();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void UpdateMaximizeButton();
    bool IsCursorOnEdge(QPoint *pMousePos);

private slots:
    void SlotMaximizeButtonClicked();
};

class FramelessWindowHelper : public QObject
{
    Q_OBJECT

private:
    bool         mbLeftBtnPressed = false;
    bool         mbCursorShapeChanged = false;
    bool         mbWidgetMovable = true;
    bool         mbWidgetResizable = true;
    bool         mbRubberBandOnResize = true;
    bool         mbRubberBandOnMove = true;

    bool         mbOnEdge = false;
    bool         mbOnEdgeLeft = false;
    bool         mbOnEdgeRight = false;
    bool         mbOnEdgeTop = false;
    bool         mbOnEdgeBottom = false;
    bool         mbOnCornerTopLeft = false;
    bool         mbOnCornerTopRight = false;
    bool         mbOnCornerBottomLeft = false;
    bool         mbOnCornerBottomRight = false;

    int          mnBorderWidth;

    QMainWindow *mpMainWindow;
    FramelessWindowToolBar *mpFramelessToolBar;
    QRubberBand *mpRubberBand;

public:
    FramelessWindowHelper(QWidget* parent);
    ~FramelessWindowHelper();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    bool HandleEventMouseMove(QObject *obj, QMouseEvent *event);
    bool HandleEventHoverMove(QObject *obj, QHoverEvent *event);
    bool HandleEventMouseButtonPress(QObject *obj, QMouseEvent *event);
    bool HandleEventMouseButtonRelease(QObject *obj, QMouseEvent *event);
    bool HandleEventMouseLeave(QObject *obj, QMouseEvent *event);
    bool HandleEventWindowTitleChange(QObject *obj, QEvent *event);
    bool HandleEventWindowIconChange(QObject *obj, QEvent *event);
    bool HandleEventWindowStateChange(QObject *obj, QWindowStateChangeEvent *event);

    void ResizeWindow(const QPoint& globalMousePos);
    void CheckCursorPosition(const QPoint& globalMousePos, const QRect& frameRect);
    void UpdateCursorShape(const QPoint& globalMousePos);

public:
    QRect GetTitleBarRect();
};
