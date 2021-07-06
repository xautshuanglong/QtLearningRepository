#pragma once

#include <QScrollBar>

class SuspendedScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit SuspendedScrollBar(QScrollBar *pShadowScrollBar, QWidget *parent = Q_NULLPTR);
    ~SuspendedScrollBar();

    void SetCustomWidth(int customWidth) { mCustomWidth = customWidth; }
    void SetCustomHeight(int customHeigt) { mCustomHeight = customHeigt; }
    void SetSibling(SuspendedScrollBar* sibling) { mpScrollBarSibling = sibling; }

private:
    void HandleEventResize(QObject *obj, QResizeEvent *event);
    void HandleMouseEnter(QObject *obj, QMouseEvent *event);
    void HandleMouseLeave(QObject *obj, QMouseEvent *event);
    void ResizeSibling();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void SlotScrollBarValueChange(int value);
    void SlotScrollBarRangeChanged(int minValue, int maxValue);

private:
    SuspendedScrollBar *mpScrollBarSibling;
    QScrollBar         *mpScrollBarShadow;
    QWidget            *mpScrollBarParent;
    QSize               mScrollBarSize;
    int                 mCustomWidth;
    int                 mCustomHeight;
};
