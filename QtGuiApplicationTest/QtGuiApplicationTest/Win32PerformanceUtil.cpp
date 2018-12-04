#include "Win32PerformanceUtil.h"

#include <windows.h>

double Win32PerformanceUtil::GetCpuUsageProcess()
{
    HANDLE hProcess = ::GetCurrentProcess();
    static DWORD s_dwTickCountOld = 0;
    static LARGE_INTEGER s_lgProcessTimeOld = { 0 };
    static DWORD s_dwProcessorCoreNum = 0;
    if (!s_dwProcessorCoreNum)
    {
        SYSTEM_INFO sysInfo = { 0 };
        GetSystemInfo(&sysInfo);
        s_dwProcessorCoreNum = sysInfo.dwNumberOfProcessors;
    }
    double dbProcCpuPercent = 0;
    BOOL bRetCode = FALSE;

    FILETIME CreateTime, ExitTime, KernelTime, UserTime;
    LARGE_INTEGER lgKernelTime;
    LARGE_INTEGER lgUserTime;
    LARGE_INTEGER lgCurTime;

    bRetCode = GetProcessTimes(hProcess, &CreateTime, &ExitTime, &KernelTime, &UserTime);
    if (bRetCode)
    {
        lgKernelTime.HighPart = KernelTime.dwHighDateTime;
        lgKernelTime.LowPart = KernelTime.dwLowDateTime;
        lgUserTime.HighPart = UserTime.dwHighDateTime;
        lgUserTime.LowPart = UserTime.dwLowDateTime;
        lgCurTime.QuadPart = (lgKernelTime.QuadPart + lgUserTime.QuadPart);
        if (s_lgProcessTimeOld.QuadPart)
        {
            DWORD dwElepsedTime = ::GetTickCount() - s_dwTickCountOld;
            dbProcCpuPercent = (double)(((double)((lgCurTime.QuadPart - s_lgProcessTimeOld.QuadPart) * 100)) / dwElepsedTime) / 10000;
            dbProcCpuPercent = dbProcCpuPercent / s_dwProcessorCoreNum;
        }
        s_lgProcessTimeOld = lgCurTime;
        s_dwTickCountOld = ::GetTickCount();
    }

    return dbProcCpuPercent;
}

double Win32PerformanceUtil::GetCpuUsageSystem()
{
    static FILETIME gPreIdleTime = { 0, 0 };
    static FILETIME gPreKernelTime = { 0, 0 };
    static FILETIME gPreUserTime = { 0, 0 };

    FILETIME curIdleTime;
    FILETIME curKernelTime;
    FILETIME curUserTime;
    ::GetSystemTimes(&curIdleTime, &curKernelTime, &curUserTime);


    long idle = CompareFileTime(&gPreIdleTime, &curIdleTime);
    long kernel = CompareFileTime(&gPreKernelTime, &curKernelTime);
    long user = CompareFileTime(&gPreUserTime, &curUserTime);


    if (kernel + user == 0)
        return 0.0;
    //（总的时间-空闲时间）/总的时间=占用cpu的时间就是使用率
    double cpuPercent = (kernel + user - idle) * 100 / (kernel + user);

    gPreIdleTime = curIdleTime;
    gPreKernelTime = curKernelTime;
    gPreUserTime = curUserTime;
    return cpuPercent;
}
