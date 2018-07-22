#ifndef TITL_EBAR_H
#define TITL_EBAR_H

#include <QToolBar>
#include <QAbstractNativeEventFilter>

class TitleBarPrivate;

class TitleBar : public QToolBar
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = Q_NULLPTR);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    void UpdateMaximizeButton();

private slots:
    void MaximizeButtonClicked();

private:
    TitleBarPrivate *m_private;
};

#endif // TITLEBAR_H
