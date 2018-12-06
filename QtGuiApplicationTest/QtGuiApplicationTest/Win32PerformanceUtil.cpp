#include "Win32PerformanceUtil.h"

#include <windows.h>
#include <Psapi.h>

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
    static bool firstFlag = true;
    if (firstFlag)
    {
        firstFlag = false;
        ::GetSystemTimes(&gPreIdleTime, &gPreKernelTime, &gPreUserTime);
    }

    FILETIME curIdleTime;
    FILETIME curKernelTime;
    FILETIME curUserTime;
    ::GetSystemTimes(&curIdleTime, &curKernelTime, &curUserTime);

    long idle = FileTimeDiff(curIdleTime, gPreIdleTime);
    long kernel = FileTimeDiff(curKernelTime, gPreKernelTime);
    long user = FileTimeDiff(curUserTime, gPreUserTime);

    if (kernel + user == 0)
        return 0.00;
    double cpuPercent = (kernel + user - idle) * 100.0 / (kernel + user);
    gPreIdleTime = curIdleTime;
    gPreKernelTime = curKernelTime;
    gPreUserTime = curUserTime;

    return cpuPercent;
}

long Win32PerformanceUtil::FileTimeDiff(FILETIME time1, FILETIME time2)
{
    __int64 value1 = time1.dwHighDateTime;
    __int64 value2 = time2.dwHighDateTime;
    value1 = value1 << 32 | time1.dwLowDateTime;
    value2 = value2 << 32 | time2.dwLowDateTime;
    long retValue = value1 - value2;
    return retValue;
}

void Win32PerformanceUtil::GetMemoryInfoSystem(ULONG &outMemLoad, ULONGLONG &outMemTotal, ULONGLONG &outMemAvailable)
{
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
    if (::GlobalMemoryStatusEx(&memoryStatus))
    {
        outMemLoad = memoryStatus.dwMemoryLoad;
        outMemTotal = memoryStatus.ullTotalPhys;
        outMemAvailable= memoryStatus.ullAvailPhys;
    }

    ULONGLONG ullTotalPhys = memoryStatus.ullTotalPhys / 1024 / 1024;
    ULONGLONG ullAvailPhys = memoryStatus.ullAvailPhys / 1024 / 1024;
    ULONGLONG ullTotalPageFile = memoryStatus.ullTotalPageFile / 1024 / 1024;
    ULONGLONG ullAvailPageFile = memoryStatus.ullAvailPageFile / 1024 / 1024;
    ULONGLONG ullTotalVirtual = memoryStatus.ullTotalVirtual / 1024 / 1024;
    ULONGLONG ullAvailVirtual = memoryStatus.ullAvailVirtual / 1024 / 1024;
    ULONGLONG ullAvailExtendedVirtual = memoryStatus.ullAvailExtendedVirtual / 1024 / 1024;

    int stop = 0;
}

void Win32PerformanceUtil::GetMemoryInfoProcess(ULONGLONG &outWorkingSetSize, ULONGLONG &outPagefileUasge)
{
    HANDLE hProcess = ::GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS processMemCounter;
    processMemCounter.cb = sizeof(processMemCounter);
    if (::GetProcessMemoryInfo(hProcess, &processMemCounter, processMemCounter.cb))
    {
        outWorkingSetSize = processMemCounter.WorkingSetSize;
        outPagefileUasge = processMemCounter.PagefileUsage;
    }

    SIZE_T peakWorkingSetSize = processMemCounter.PeakWorkingSetSize / 1024;
    SIZE_T workingSetSize = processMemCounter.WorkingSetSize / 1024;
    SIZE_T quotaPeakPagedPoolUsage = processMemCounter.QuotaPeakPagedPoolUsage / 1024;
    SIZE_T quotaPagedPoolUsage = processMemCounter.QuotaPagedPoolUsage / 1024;
    SIZE_T quotaPeakNonPagedPoolUsage = processMemCounter.QuotaPeakNonPagedPoolUsage / 1024;
    SIZE_T quotaNonPagedPoolUsage = processMemCounter.QuotaNonPagedPoolUsage / 1024;
    SIZE_T pagefileUsage = processMemCounter.PagefileUsage / 1024;
    SIZE_T peakPagefileUsage = processMemCounter.PeakPagefileUsage / 1024;

    int stop = 0;
}
