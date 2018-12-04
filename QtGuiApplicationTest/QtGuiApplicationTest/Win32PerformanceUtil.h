#pragma once

#include <windows.h>

class Win32PerformanceUtil
{
public:
    Win32PerformanceUtil() = delete;
    ~Win32PerformanceUtil() = delete;

    static double GetCpuUsageProcess();
    static double GetCpuUsageSystem();

private:
    static long FileTimeDiff(FILETIME time1, FILETIME time2);
};

