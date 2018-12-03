#pragma once

#include <QWidget>
#include <QString>

class QRubberBand;
class QToolButton;
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
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QWidget        *mpWidgetParent;
    QRubberBand    *mpRubberBand;
    QToolButton    *mpBtnMinimum;
    QToolButton    *mpBtnMaximum;
    QToolButton    *mpBtnClose;
    QRect          *mpRectNormalWindow;
    int             mnBorderWidth;
    bool            mbRubberBandOnResize;
    bool            mbLeftBtnPressed;
    bool            mbCursorShapeChanged;
    bool            mbOnCornerTopLeft;
    bool            mbOnCornerTopRight;
    bool            mbOnCornerBottomLeft;
    bool            mbOnCornerBottomRight;
    bool            mbOnEdge;
    bool            mbOnEdgeLeft;
    bool            mbOnEdgeRight;
    bool            mbOnEdgeTop;
    bool            mbOnEdgeBottom;
};
