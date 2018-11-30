#pragma once

#include <QEvent>

/************************************************************************/
/* 调试模块事件定义：调试面板菜单事件处理。                                */
/************************************************************************/
class DebugEvent : public QEvent
{
public:
    enum MenuID
    {
        // 文件菜单
        FILE_SAVE,
        FILE_SAVE_AS,
        // 视图菜单
        VIEW_LIST,
        VIEW_TAB,
        // 设置菜单
        SETTING_DEBUG_LIST
    };

public:
    DebugEvent();
    ~DebugEvent();
    static Type StaticEventType();
    void SetMenuID(MenuID id) { mMenuID = id; };
    MenuID GetMenuID() { return mMenuID; };

private:
    static const Type mgEventType;
    MenuID mMenuID;
};

