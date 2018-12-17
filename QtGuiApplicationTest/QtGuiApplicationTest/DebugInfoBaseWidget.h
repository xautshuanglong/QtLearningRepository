#ifndef DEBUG_INFO_BASE_WIDGET_H
#define DEBUG_INFO_BASE_WIDGET_H

#include <QWidget>

class DebugMenuEvent;

class DebugInfoBaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugInfoBaseWidget(QWidget *parent = 0);
    ~DebugInfoBaseWidget();

    virtual bool OnDebugMenuEvent(DebugMenuEvent *event);
    virtual void OnUpdateDebugInfo();
};

#endif // DEBUG_INFO_BASE_WIDGET_H
