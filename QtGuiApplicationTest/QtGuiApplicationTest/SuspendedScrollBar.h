#pragma once

#include <QScrollBar>

class SuspendedScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit SuspendedScrollBar(QScrollBar *pShadowScrollBar, QWidget *parent = Q_NULLPTR);
    //explicit SuspendedScrollBar(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR);
    ~SuspendedScrollBar();

private:
    void HandleEventResize(QObject *obj, QResizeEvent *event);
    void HandleMouseEnter(QObject *obj, QMouseEvent *event);
    void HandleMouseLeave(QObject *obj, QMouseEvent *event);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void SlotScrollBarValueChange(int value);
    void SlotScrollBarRangeChanged(int minValue, int maxValue);

private:
    QScrollBar   *mpScrollBarShadow;
};
