#pragma once

#include <windows.h>
#include <QString>

class Win32PerformanceUtil
{
public:
    typedef struct tagTcpConnections
    {
        quint16  localPort;
        quint16  remotePort;
        QString localAddress;
        QString remoteAddress;
        QString status;
    }TcpConnections;

    typedef struct tagUdpConnections
    {
        quint16 localPort;
        QString localAddress;
    }UdpConnections;

public:
    Win32PerformanceUtil() = delete;
    ~Win32PerformanceUtil() = delete;

    static bool GetCpuUsageProcess(double &outCpuPercent);
    static double GetCpuUsageSystem();
    static void GetMemoryInfoSystem(ULONG &outMemLoad, ULONGLONG &outMemTotal, ULONGLONG &outMemAvailable);
    static void GetMemoryInfoProcess(ULONGLONG &outWorkingSetSize, ULONGLONG &outPagefileUasge);
    static void GetTcpStaticsInfo(ULONG inProtoclFamily);
    static void GetUdpStaticsInfo(ULONG inProtoclFamily);
    static void GetExtendTcpTableInfo(ULONG inProtoclFamily, QList<TcpConnections> &outListConnections);
    static void GetExtendUdpTableInfo(ULONG inProtoclFamily, QList<UdpConnections> &outListConnections);
    static QString GetTcpState(DWORD state);

private:
    static long long FileTimeDiff(FILETIME time1, FILETIME time2);
};

