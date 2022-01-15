
#ifndef QTLIBCORE_HOOK_PROCEDURE_H
#define QTLIBCORE_HOOK_PROCEDURE_H

#include "qtlibrarycoretest_global.h"

#include <Windows.h>

class QTLIBRARYCORETEST_EXPORT HookProcedure final
{
public:
    HookProcedure() = delete;
    ~HookProcedure() = delete;

    static void SetModuleHandle(HINSTANCE hMod);
    static void InstallHook();
    static void UninstallHook();

    static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK DebugProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK MessageProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    static HINSTANCE gModuleHandle;
};

#endif // QTLIBCORE_HOOK_PROCEDURE_H
