#include <windows.h>

#include "HookProcedure.h"

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HookProcedure::SetModuleHandle(hModule);
        break;
    case DLL_PROCESS_DETACH:
        HookProcedure::UninstallHook();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }
    return TRUE;
}
