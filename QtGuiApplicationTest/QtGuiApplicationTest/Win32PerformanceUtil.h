#pragma once
class Win32PerformanceUtil
{
public:
    Win32PerformanceUtil() = delete;
    ~Win32PerformanceUtil() = delete;

    static double GetCpuUsageProcess();
    static double GetCpuUsageSystem();
};

