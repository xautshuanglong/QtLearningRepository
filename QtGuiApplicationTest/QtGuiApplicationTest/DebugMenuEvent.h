#pragma once

#include <QEvent>

/************************************************************************/
/* 调试模块事件定义：调试面板菜单事件处理。                                */
/************************************************************************/
class DebugMenuEvent : public QEvent
{
public:
    DebugMenuEvent();
    ~DebugMenuEvent();
    static Type StaticEventType();
    void SetMenuID(int id) { mMenuID = id; };
    int GetMenuID() { return mMenuID; };

private:
    static const Type mgEventType;
    int mMenuID;
};

