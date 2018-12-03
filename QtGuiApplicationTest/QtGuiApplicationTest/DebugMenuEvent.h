#pragma once

#include <QEvent>

/************************************************************************/
/* ����ģ���¼����壺�������˵��¼�����                                */
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

