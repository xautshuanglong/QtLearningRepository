#include <windows.h>

//#include <LogUtil.h>

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
//#ifdef _DEBUG
//        LogUtil::Init(LOG_LEVEL_DEBUG);
//#else
//        LogUtil::Init(LOG_LEVEL_INFO);
//#endif
//        LogUtil::Debug(CODE_LOCATION, "---------- QtLibarayCoreTest DLL_PROCESS_ATTACH ----------");
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }
    return TRUE;
}
