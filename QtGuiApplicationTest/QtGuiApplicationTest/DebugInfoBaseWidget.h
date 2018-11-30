#ifndef DEBUG_INFO_BASE_WIDGET_H
#define DEBUG_INFO_BASE_WIDGET_H

#include <QWidget>

class DebugEvent;

class DebugInfoBaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugInfoBaseWidget(QWidget *parent = 0);
    ~DebugInfoBaseWidget();

protected:
    virtual bool DebugMenuEvent(DebugEvent *event);
};

#endif // DEBUG_INFO_BASE_WIDGET_H
