#pragma once

#include <windows.h>

class Win32PerformanceUtil
{
public:
    Win32PerformanceUtil() = delete;
    ~Win32PerformanceUtil() = delete;

    static double GetCpuUsageProcess();
    static double GetCpuUsageSystem();
    static void GetMemoryInfoSystem(ULONGLONG &outMemTotal, ULONGLONG &outMemUsed);
    static void GetMemoryInfoProcess(ULONGLONG &outMemUsed);

private:
    static long FileTimeDiff(FILETIME time1, FILETIME time2);
};

