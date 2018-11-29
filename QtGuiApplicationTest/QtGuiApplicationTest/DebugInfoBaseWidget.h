#ifndef DEBUG_INFO_BASE_WIDGET_H
#define DEBUG_INFO_BASE_WIDGET_H

#include <QWidget>

class DebugInfoBaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugInfoBaseWidget(QWidget *parent = 0);
    ~DebugInfoBaseWidget();
};

#endif // DEBUG_INFO_BASE_WIDGET_H
