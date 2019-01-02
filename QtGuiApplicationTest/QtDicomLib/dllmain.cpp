#include "qtdicomlib_global.h"

// Self Headers
#include <LogUtil.h>

#ifdef WIN32

#include <windows.h>

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#endif // WIN32

QTDICOMLIB_EXPORT void InitializeEntry()
{
    //TODO: initialize something
#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG);
#else
    LogUtil::Init(LOG_LEVEL_INFO);
#endif
}

QTDICOMLIB_EXPORT void UnInitializeEntry()
{
    //TODO: uninitialize something
}
