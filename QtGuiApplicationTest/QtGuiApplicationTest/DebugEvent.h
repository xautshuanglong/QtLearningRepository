#pragma once

#include <QEvent>

/************************************************************************/
/* ����ģ���¼����壺�������˵��¼�����                                */
/************************************************************************/
class DebugEvent : public QEvent
{
public:
    enum MenuID
    {
        // �ļ��˵�
        FILE_SAVE,
        FILE_SAVE_AS,
        // ��ͼ�˵�
        VIEW_LIST,
        VIEW_TAB,
        // ���ò˵�
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

