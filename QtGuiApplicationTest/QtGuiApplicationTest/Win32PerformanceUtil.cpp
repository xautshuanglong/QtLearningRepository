#include "Win32PerformanceUtil.h"

// Win32 Headers
#include <Psapi.h>
#include <IPHlpApi.h>

// QT
#include <QList>

#include <LogUtil.h>

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

    //ULONGLONG ullTotalPhys = memoryStatus.ullTotalPhys / 1024 / 1024;
    //ULONGLONG ullAvailPhys = memoryStatus.ullAvailPhys / 1024 / 1024;
    //ULONGLONG ullTotalPageFile = memoryStatus.ullTotalPageFile / 1024 / 1024;
    //ULONGLONG ullAvailPageFile = memoryStatus.ullAvailPageFile / 1024 / 1024;
    //ULONGLONG ullTotalVirtual = memoryStatus.ullTotalVirtual / 1024 / 1024;
    //ULONGLONG ullAvailVirtual = memoryStatus.ullAvailVirtual / 1024 / 1024;
    //ULONGLONG ullAvailExtendedVirtual = memoryStatus.ullAvailExtendedVirtual / 1024 / 1024;
    //int stop = 0;
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

    //SIZE_T peakWorkingSetSize = processMemCounter.PeakWorkingSetSize / 1024;
    //SIZE_T workingSetSize = processMemCounter.WorkingSetSize / 1024;
    //SIZE_T quotaPeakPagedPoolUsage = processMemCounter.QuotaPeakPagedPoolUsage / 1024;
    //SIZE_T quotaPagedPoolUsage = processMemCounter.QuotaPagedPoolUsage / 1024;
    //SIZE_T quotaPeakNonPagedPoolUsage = processMemCounter.QuotaPeakNonPagedPoolUsage / 1024;
    //SIZE_T quotaNonPagedPoolUsage = processMemCounter.QuotaNonPagedPoolUsage / 1024;
    //SIZE_T pagefileUsage = processMemCounter.PagefileUsage / 1024;
    //SIZE_T peakPagefileUsage = processMemCounter.PeakPagefileUsage / 1024;
    //int stop = 0;
}

void Win32PerformanceUtil::GetTcpStaticsInfo(ULONG inProtoclFamily)
{
    MIB_TCPSTATS mibTcpStatistics;
    DWORD dwRetFlag = ::GetTcpStatisticsEx(&mibTcpStatistics, inProtoclFamily);
    if (dwRetFlag == NO_ERROR)
    {
        int test = 0;
    }
    else if (dwRetFlag == ERROR_INVALID_PARAMETER)
    {
        LogUtil::Debug(CODE_LOCATION, "The pStats parameter is NULL or does not point to valid memory, or the dwFamily parameter is not a valid value.");
    }
    else if (dwRetFlag == ERROR_NOT_SUPPORTED)
    {
        LogUtil::Debug(CODE_LOCATION, "This function is not supported on the operating system on which the function call was made.");
    }
    else
    {
        LPVOID lpMsgBuf = NULL;
        if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL, dwRetFlag, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)lpMsgBuf, 0, NULL))
        {
            LogUtil::Debug(CODE_LOCATION, "GetTcpStatisticsEx Error: %s", lpMsgBuf);
        }
        ::LocalFree(lpMsgBuf);
    }
}

void Win32PerformanceUtil::GetUdpStaticsInfo(ULONG inProtoclFamily)
{
    MIB_UDPSTATS mibUdpStatistics;
    DWORD dwRetFlag = ::GetUdpStatisticsEx(&mibUdpStatistics, inProtoclFamily);
    if (dwRetFlag == NO_ERROR)
    {
        int test = 0;
    }
    else if (dwRetFlag == ERROR_INVALID_PARAMETER)
    {
        LogUtil::Error(CODE_LOCATION, "The pStats parameter is NULL or does not point to valid memory, or the dwFamily parameter is not a valid value.");
    }
    else if (dwRetFlag == ERROR_NOT_SUPPORTED)
    {
        LogUtil::Error(CODE_LOCATION, "This function is not supported on the operating system on which the function call was made.");
    }
    else
    {
        LPVOID lpMsgBuf = NULL;
        if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL, dwRetFlag, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            (LPTSTR)lpMsgBuf, 0, NULL))
        {
            LogUtil::Error(CODE_LOCATION, "GetTcpStatisticsEx Error: %s", lpMsgBuf);
        }
        ::LocalFree(lpMsgBuf);
    }
}

void Win32PerformanceUtil::GetExtendTcpTableInfo(ULONG inProtoclFamily, QList<TcpConnections> &outListConnections)
{
    PMIB_TCPTABLE_OWNER_PID pTcpTable = NULL;
    DWORD  dwSize = 0;
    DWORD retFlag = ::GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, inProtoclFamily, TCP_TABLE_OWNER_PID_ALL, 0);
    pTcpTable = (PMIB_TCPTABLE_OWNER_PID)new char[dwSize];

    retFlag = ::GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, inProtoclFamily, TCP_TABLE_OWNER_PID_ALL, 0);
    if (retFlag == NO_ERROR)
    {
        DWORD currentPID = ::GetCurrentProcessId();
        DWORD dwNumEntries = pTcpTable->dwNumEntries;
        IN_ADDR tempAddrLocal, tempAddrRemote;
        TcpConnections tempConnection;
        for (DWORD index=0; index<dwNumEntries; ++index)
        {
            if (currentPID == pTcpTable->table[index].dwOwningPid)
            {
                tempAddrLocal.S_un.S_addr = pTcpTable->table[index].dwLocalAddr;
                tempAddrRemote.S_un.S_addr = pTcpTable->table[index].dwRemoteAddr;

                tempConnection.localAddress = ::inet_ntoa(tempAddrLocal);
                tempConnection.remoteAddress = ::inet_ntoa(tempAddrRemote);
                tempConnection.localPort = ::ntohs((unsigned short)(0x0000FFFF & pTcpTable->table[index].dwLocalPort));
                tempConnection.remotePort = ::ntohs((unsigned short)(0x0000FFFF & pTcpTable->table[index].dwRemotePort));
                tempConnection.status = GetTcpState(pTcpTable->table[index].dwState);
                outListConnections.append(tempConnection);

                //LogUtil::Debug(CODE_LOCATION, "PID: %d    Local: %s::%d    Remote: %s::%d    State: %d",
                //               pTcpTable->table[index].dwOwningPid,
                //               ::inet_ntoa(tempAddrLocal),
                //               ::ntohs((unsigned short)(0x0000FFFF & pTcpTable->table[index].dwLocalPort)),
                //               ::inet_ntoa(tempAddrRemote),
                //               ::ntohs((unsigned short)(0x0000FFFF & pTcpTable->table[index].dwRemotePort)),
                //               GetTcpState(pTcpTable->table[index].dwState)
                //);
            }
        }
    }
    else if (retFlag == ERROR_INSUFFICIENT_BUFFER)
    {
        LogUtil::Error(CODE_LOCATION, "An insufficient amount of space was allocated for the table.");
    }
    else if (retFlag == ERROR_INVALID_PARAMETER)
    {
        LogUtil::Error(CODE_LOCATION, "An invalid parameter was passed to the function.");
    }

    delete pTcpTable;
}

void Win32PerformanceUtil::GetExtendUdpTableInfo(ULONG inProtoclFamily, QList<UdpConnections> &outListConnections)
{
    PMIB_UDPTABLE_OWNER_PID pUdpTable = NULL;
    DWORD  dwSize = 0;
    DWORD retFlag = ::GetExtendedUdpTable(pUdpTable, &dwSize, TRUE, inProtoclFamily, UDP_TABLE_OWNER_PID, 0);
    pUdpTable = (PMIB_UDPTABLE_OWNER_PID)new char[dwSize];

    retFlag = ::GetExtendedUdpTable(pUdpTable, &dwSize, TRUE, inProtoclFamily, UDP_TABLE_OWNER_PID, 0);
    if (retFlag == NO_ERROR)
    {
        DWORD currentPID = ::GetCurrentProcessId();
        DWORD dwNumEntries = pUdpTable->dwNumEntries;
        IN_ADDR tempAddrLocal;
        UdpConnections tempConnection;
        for (DWORD index = 0; index < dwNumEntries; ++index)
        {
            if (currentPID == pUdpTable->table[index].dwOwningPid)
            {
                tempAddrLocal.S_un.S_addr = pUdpTable->table[index].dwLocalAddr;

                tempConnection.localAddress = ::inet_ntoa(tempAddrLocal);
                tempConnection.localPort = ::ntohs((unsigned short)(0x0000FFFF & pUdpTable->table[index].dwLocalPort));
                outListConnections.append(tempConnection);

                LogUtil::Debug(CODE_LOCATION, "PID: %d    Local: %s::%d",
                               pUdpTable->table[index].dwOwningPid,
                               ::inet_ntoa(tempAddrLocal),
                               ::ntohs((unsigned short)(0x0000FFFF & pUdpTable->table[index].dwLocalPort))
                );
            }
        }
    }
    else if (retFlag == ERROR_INSUFFICIENT_BUFFER)
    {
        LogUtil::Error(CODE_LOCATION, "An insufficient amount of space was allocated for the table.");
    }
    else if (retFlag == ERROR_INVALID_PARAMETER)
    {
        LogUtil::Error(CODE_LOCATION, "An invalid parameter was passed to the function.");
    }

    delete pUdpTable;
}

QString Win32PerformanceUtil::GetTcpState(DWORD state)
{
    QString retValue = "UNKNOWN";
    switch (state)
    {
    case MIB_TCP_STATE_CLOSED:
        retValue = "MIB_TCP_STATE_CLOSED";
        break;
    case MIB_TCP_STATE_LISTEN:
        retValue = "MIB_TCP_STATE_LISTEN";
        break;
    case MIB_TCP_STATE_SYN_SENT:
        retValue = "MIB_TCP_STATE_SYN_SENT";
        break;
    case MIB_TCP_STATE_SYN_RCVD:
        retValue = "MIB_TCP_STATE_SYN_RCVD";
        break;
    case MIB_TCP_STATE_ESTAB:
            retValue = "MIB_TCP_STATE_ESTAB";
        break;
    case MIB_TCP_STATE_FIN_WAIT1:
        retValue = "MIB_TCP_STATE_FIN_WAIT1";
        break;
    case MIB_TCP_STATE_FIN_WAIT2:
        retValue = "MIB_TCP_STATE_FIN_WAIT2";
        break;
    case MIB_TCP_STATE_CLOSE_WAIT:
        retValue = "MIB_TCP_STATE_CLOSE_WAIT";
        break;
    case MIB_TCP_STATE_CLOSING:
        retValue = "MIB_TCP_STATE_CLOSING";
        break;
    case MIB_TCP_STATE_LAST_ACK:
        retValue = "MIB_TCP_STATE_LAST_ACK";
        break;
    case MIB_TCP_STATE_TIME_WAIT:
        retValue = "MIB_TCP_STATE_TIME_WAIT";
        break;
    case MIB_TCP_STATE_DELETE_TCB:
        retValue = "MIB_TCP_STATE_DELETE_TCB";
        break;
    default:
        break;
    }
    return retValue;
}
