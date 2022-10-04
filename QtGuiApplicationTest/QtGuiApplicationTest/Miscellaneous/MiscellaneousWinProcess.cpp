#include "MiscellaneousWinProcess.h"
#include "ui_MiscellaneousWinProcess.h"

#include <windows.h>
#include <tchar.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include "JCB_Logger/LogUtil.h"

MiscellaneousWinProcess::MiscellaneousWinProcess(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousWinProcess())
{
    ui->setupUi(this);
}

MiscellaneousWinProcess::~MiscellaneousWinProcess()
{
    delete ui;
}

QString MiscellaneousWinProcess::GetTitle()
{
    return QObject::tr("Process");
}

QString MiscellaneousWinProcess::GetTitleTooltip()
{
    return QObject::tr("How to operate process and some testing base on process.");
}

MiscellaneousTestGroup MiscellaneousWinProcess::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinProcess::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Process;
}

void MiscellaneousWinProcess::on_btnEnumProcess_clicked()
{
    //this->PSAPI_EnumProcess();
    this->TLAPI_ProcessSnapshot();
}

void MiscellaneousWinProcess::on_btnEnumModule_clicked()
{
    int i = 0;
}

// https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
void MiscellaneousWinProcess::PSAPI_EnumProcess()
{
    const DWORD outPidCntStep = 100;
    DWORD predictPidCnt = 0;
    DWORD outBufSize = 0;
    DWORD cbNeeded = 0;
    DWORD *pOutProcPids = nullptr;

    do
    {
        predictPidCnt += outPidCntStep;
        outBufSize = predictPidCnt * sizeof(DWORD);
        if (pOutProcPids)
        {
            delete[] pOutProcPids;
            pOutProcPids = nullptr;
        }
        pOutProcPids = new DWORD[predictPidCnt];
        BOOL resFlag = ::EnumProcesses(pOutProcPids, outBufSize, &cbNeeded);
        if (!resFlag)
        {
            DWORD errCode = ::GetLastError();
            LogUtil::Error(CODE_LOCATION, "EnumProcesses failed! ErrorCode=%08X", errCode);
            cbNeeded = 0;
            break;
        }
    } while (cbNeeded == outBufSize);
    if (cbNeeded > 0)
    {
        DWORD realPidCount = cbNeeded / sizeof(DWORD);
        for (DWORD i = 0; i < realPidCount; ++i)
        {
            this->PSAPI_PrintProcessName(pOutProcPids[i]);
            this->PSAPI_PrintProcessModule(pOutProcPids[i]);
            //LogUtil::Debug(CODE_LOCATION, "PIDS[%u]=%u", i, pOutProcPids[i]);
        }
    }

    if (pOutProcPids)
    {
        delete[] pOutProcPids;
        pOutProcPids = nullptr;
    }
}

// https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
void MiscellaneousWinProcess::PSAPI_PrintProcessName(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.
    // If the specified process is the System Idle Process (0x00000000), the function fails and the last error code is ERROR_INVALID_PARAMETER.
    // If the specified process is the System process or one of the Client Server Run-Time Subsystem (CSRSS) processes,
    // this function fails and the last error code is ERROR_ACCESS_DENIED because their access restrictions prevent user-level code from opening them.
    //HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID); // 存在一部分进程拒绝访问，错误码 0x00000005
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    // Get the process name.
    if (NULL != hProcess)
    {
        HMODULE hMod = NULL;
        DWORD cbNeeded = 0;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
        {
            DWORD copiedLen = GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
            if (copiedLen == 0)
            {
                LogUtil::Error(CODE_LOCATION, "GetModuleBaseName failed! ErrorCode=0x%08X", ::GetLastError());
            }
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "EnumProcessModules failed! ErrorCode=0x%08X", ::GetLastError());
        }

        LogUtil::Debug(CODE_LOCATION, "PID:%-5u ProcessName:%s", processID, szProcessName);

        // Release the handle to the process.
        CloseHandle(hProcess);
        hProcess = NULL;
    }
    else
    {
        LogUtil::Debug(CODE_LOCATION, "PID:%-5u ProcessName:%s  -- OpenProcess failed! ErrorCode=0x%08X",
            processID, szProcessName, ::GetLastError());
    }

    // Print the process name and identifier.
    //_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
}

// https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-modules-for-a-process
void MiscellaneousWinProcess::PSAPI_PrintProcessModule(DWORD processID)
{
    // Get a handle to the process.
    // If the specified process is the System Idle Process (0x00000000), the function fails and the last error code is ERROR_INVALID_PARAMETER.
    // If the specified process is the System process or one of the Client Server Run-Time Subsystem (CSRSS) processes,
    // this function fails and the last error code is ERROR_ACCESS_DENIED because their access restrictions prevent user-level code from opening them.
    //HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID); // 存在一部分进程拒绝访问，错误码 0x00000005
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    // Get the process name.
    if (NULL != hProcess)
    {
        const DWORD predictStep = 100;
        HMODULE *pModuleList = nullptr;
        TCHAR moduleName[MAX_PATH] = TEXT("<unknown>");
        DWORD cbNeeded = 0;
        DWORD predictCount = predictStep;

        // Do not call CloseHandle on any of the handles returned by this function.
        // The information comes from a snapshot, so there are no resources to be freed.
        BOOL modResFlag = EnumProcessModules(hProcess, pModuleList, 0, &cbNeeded);
        if (modResFlag)
        {
            if (cbNeeded > 0)
            {
                DWORD modRealCount = cbNeeded / sizeof(HMODULE);
                pModuleList = new HMODULE[modRealCount];
                modResFlag = EnumProcessModules(hProcess, pModuleList, cbNeeded, &cbNeeded);
                if (modResFlag)
                {
                    for (DWORD i = 0; i < modRealCount; ++i)
                    {
                        DWORD copiedLen = GetModuleFileNameEx(hProcess, pModuleList[i], moduleName, sizeof(moduleName) / sizeof(TCHAR));
                        if (copiedLen > 0)
                        {
                            LogUtil::Debug(CODE_LOCATION, "PID:%-5u ModuleName=%s", processID, moduleName);
                        }
                        else
                        {
                            LogUtil::Error(CODE_LOCATION, "GetModuleBaseName failed! ErrorCode=0x%08X", ::GetLastError());
                        }
                    }
                }
                else
                {
                    LogUtil::Error(CODE_LOCATION, "EnumProcessModules failed! ErrorCode=0x%08X", ::GetLastError());
                }
            }
            else
            {
                LogUtil::Debug(CODE_LOCATION, "There are no modules.");
            }
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "EnumProcessModules failed! ErrorCode=0x%08X", ::GetLastError());
        }

        //LogUtil::Debug(CODE_LOCATION, "PID:%-5u ProcessName:%s", processID, szProcessName);

        // Release the handle to the process.
        CloseHandle(hProcess);
        hProcess = NULL;
    }
    else
    {
        LogUtil::Debug(CODE_LOCATION, "PID:%-5u -- OpenProcess failed! ErrorCode=0x%08X", processID, ::GetLastError());
    }

    // Print the process name and identifier.
    //_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
}

// https://learn.microsoft.com/en-us/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes
void MiscellaneousWinProcess::TLAPI_ProcessSnapshot()
{
    HANDLE hProcessSnap = nullptr;
    HANDLE hProcess = nullptr;
    PROCESSENTRY32 procEntry32 = { 0 };
    DWORD dwPriorityClass = 0;

    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        LogUtil::Error(CODE_LOCATION, "CreateToolhelp32Snapshot failed! ErrorCode=0x%08X", ::GetLastError());
        return;
    }

    procEntry32.dwSize = sizeof(PROCESSENTRY32);
    BOOL procWalkRes = ::Process32First(hProcessSnap, &procEntry32);
    if (!procWalkRes)
    {
        LogUtil::Error(CODE_LOCATION, "Process32First failed! ErrorCode=0x%08X", ::GetLastError());
    }
    do 
    {
        LogUtil::Debug(CODE_LOCATION, "-------------------------------------------------------");
        LogUtil::Debug(CODE_LOCATION, "ExeFilename   : %s", procEntry32.szExeFile);
        LogUtil::Debug(CODE_LOCATION, "ParentID      : %u", procEntry32.th32ParentProcessID);
        LogUtil::Debug(CODE_LOCATION, "ProcessID     : %u", procEntry32.th32ProcessID);
        LogUtil::Debug(CODE_LOCATION, "ThreadCount   : %u", procEntry32.cntThreads);
        LogUtil::Debug(CODE_LOCATION, "PriClassBase  : %ld", procEntry32.pcPriClassBase);

        hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, procEntry32.th32ProcessID);
        if (hProcess != NULL)
        {
            DWORD priorityClass = ::GetPriorityClass(hProcess);
            LogUtil::Debug(CODE_LOCATION, "PriorityClass : 0x%08X", priorityClass);
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "OpenProcess failed! ErrorCode=0x%08X", ::GetLastError());
        }

        procWalkRes = ::Process32Next(hProcessSnap, &procEntry32);
        if (!procWalkRes)
        {
            DWORD errCode = ::GetLastError();
            if (errCode != ERROR_NO_MORE_FILES)
            {
                LogUtil::Error(CODE_LOCATION, "Process32Next failed! ErrorCode=0x%08X", ::GetLastError());
            }
        }
    } while (procWalkRes);
}
