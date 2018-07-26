#pragma once

#include <QObject>

class FramelessWindowHelperPrivate;

class FramelessWindowHelper : public QObject
{
    Q_OBJECT

public:
    FramelessWindowHelper(QWidget* parent);
    ~FramelessWindowHelper();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    bool HandleEventMouseMove(QObject *obj, QEvent *event);
    bool HandleEventHoverMove(QObject *obj, QEvent *event);
    bool HandleEventMouseButtonPress(QObject *obj, QEvent *event);
    bool HandleEventMouseButtonRelease(QObject *obj, QEvent *event);
    bool HandleEventMouseLeave(QObject *obj, QEvent *event);
    bool HandleEventWindowTitleChange(QObject *obj, QEvent *event);
    bool HandleEventWindowIconChange(QObject *obj, QEvent *event);
    bool HandleEventWindowStateChange(QObject *obj, QEvent *event);
};
