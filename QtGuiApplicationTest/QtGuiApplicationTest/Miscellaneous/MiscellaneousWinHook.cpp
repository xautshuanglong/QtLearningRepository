#include "MiscellaneousWinHook.h"
#include "ui_MiscellaneousWinHook.h"

#include <windows.h>
#include <tchar.h>

#include "JCB_Logger/LogUtil.h"

MiscellaneousWinHook::MiscellaneousWinHook(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousWinHook())
    , mIsMonitorRegRunning(false)
    , mpRegMonitorThread(nullptr)
{
    ui->setupUi(this);
}

MiscellaneousWinHook::~MiscellaneousWinHook()
{
    delete ui;
}

QString MiscellaneousWinHook::GetTitle()
{
    return QObject::tr("Hook");
}

QString MiscellaneousWinHook::GetTitleTooltip()
{
    return QObject::tr("How to operate hook and monitor events.");
}

MiscellaneousTestGroup MiscellaneousWinHook::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinHook::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Hook;
}

void MiscellaneousWinHook::on_btnWindowsHookStart_clicked()
{
    int i = 0;
}

void MiscellaneousWinHook::on_btnWindowsHookStop_clicked()
{
    int i = 0;
}

void MiscellaneousWinHook::MonitorRegChange_RegNotifyChangeValue(const std::string& mainKey, const std::string& subKey)
{

    DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME |
        REG_NOTIFY_CHANGE_ATTRIBUTES |
        REG_NOTIFY_CHANGE_LAST_SET |
        REG_NOTIFY_CHANGE_SECURITY;

    HANDLE hEvent;
    HKEY   hMainKey;
    HKEY   hKey;
    LONG   lErrorCode;

    // Convert parameters to appropriate handles.
    if (_tcscmp(TEXT("HKLM"), mainKey.c_str()) == 0) hMainKey = HKEY_LOCAL_MACHINE;
    else if (_tcscmp(TEXT("HKU"), mainKey.c_str()) == 0) hMainKey = HKEY_USERS;
    else if (_tcscmp(TEXT("HKCU"), mainKey.c_str()) == 0) hMainKey = HKEY_CURRENT_USER;
    else if (_tcscmp(TEXT("HKCR"), mainKey.c_str()) == 0) hMainKey = HKEY_CLASSES_ROOT;
    else if (_tcscmp(TEXT("HCC"), mainKey.c_str()) == 0) hMainKey = HKEY_CURRENT_CONFIG;
    else
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Usage: notify [HKLM|HKU|HKCU|HKCR|HCC] [<subkey>]"));
        return;
    }

    // Open a key.
    lErrorCode = RegOpenKeyEx(hMainKey, subKey.c_str(), 0, KEY_NOTIFY, &hKey);
    if (lErrorCode != ERROR_SUCCESS)
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegOpenKeyEx (%d)."), lErrorCode);
        return;
    }

    // Create an event.
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL)
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in CreateEvent (%d)."), GetLastError());
        return;
    }

    // Watch the registry key for a change of value.
    lErrorCode = RegNotifyChangeKeyValue(hKey,
        TRUE,
        dwFilter,
        hEvent,
        TRUE);
    if (lErrorCode != ERROR_SUCCESS)
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegNotifyChangeKeyValue (%d)."), lErrorCode);
        return;
    }

    // Wait for an event to occur.
    LogUtil::Debug(CODE_LOCATION, TEXT("Waiting for a change in the specified key..."));
    if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in WaitForSingleObject (%d)."), GetLastError());
        return;
    }
    else
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Change has occurred."));
    }

    // Close the key.
    lErrorCode = RegCloseKey(hKey);
    if (lErrorCode != ERROR_SUCCESS)
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegCloseKey (%d)."), GetLastError());
        return;
    }

    // Close the handle.
    if (!CloseHandle(hEvent))
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in CloseHandle."));
        return;
    }
}


void MiscellaneousWinHook::MonitorRegChange_RegNotifyChangeValueLoop(const std::string& mainKey, const std::string& subKey)
{

    DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME |
        REG_NOTIFY_CHANGE_ATTRIBUTES |
        REG_NOTIFY_CHANGE_LAST_SET |
        REG_NOTIFY_CHANGE_SECURITY;

    HANDLE hEvent;
    HKEY   hMainKey;
    HKEY   hKey;
    LONG   lErrorCode;

    // Convert parameters to appropriate handles.
    if (_tcscmp(TEXT("HKLM"), mainKey.c_str()) == 0) hMainKey = HKEY_LOCAL_MACHINE;
    else if (_tcscmp(TEXT("HKU"), mainKey.c_str()) == 0) hMainKey = HKEY_USERS;
    else if (_tcscmp(TEXT("HKCU"), mainKey.c_str()) == 0) hMainKey = HKEY_CURRENT_USER;
    else if (_tcscmp(TEXT("HKCR"), mainKey.c_str()) == 0) hMainKey = HKEY_CLASSES_ROOT;
    else if (_tcscmp(TEXT("HCC"), mainKey.c_str()) == 0) hMainKey = HKEY_CURRENT_CONFIG;
    else
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Usage: notify [HKLM|HKU|HKCU|HKCR|HCC] [<subkey>]"));
        return;
    }

    // Open a key.
    lErrorCode = RegOpenKeyEx(hMainKey, subKey.c_str(), 0, KEY_NOTIFY, &hKey);
    if (lErrorCode != ERROR_SUCCESS)
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegOpenKeyEx (%d)."), lErrorCode);
        return;
    }

    // Create an event.
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL)
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in CreateEvent (%d)."), GetLastError());
        return;
    }

    int monitorCount = 0;
    do 
    {
        ++monitorCount;

        // Watch the registry key for a change of value.
        lErrorCode = RegNotifyChangeKeyValue(hKey,
            TRUE,
            dwFilter,
            hEvent,
            TRUE);
        if (lErrorCode != ERROR_SUCCESS)
        {
            LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegNotifyChangeKeyValue (%d)."), lErrorCode);
            return;
        }

        // Wait for an event to occur.
        LogUtil::Debug(CODE_LOCATION, TEXT("Waiting for a change in the specified key..."));
        if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)
        {
            LogUtil::Debug(CODE_LOCATION, TEXT("Error in WaitForSingleObject (%d)."), GetLastError());
            return;
        }
        else
        {
            LogUtil::Debug(CODE_LOCATION, TEXT("Change has occurred."));
        }
    } while (monitorCount <= 10);

    // Close the key.
    lErrorCode = RegCloseKey(hKey);
    if (lErrorCode != ERROR_SUCCESS)
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegCloseKey (%d)."), GetLastError());
        return;
    }

    // Close the handle.
    if (!CloseHandle(hEvent))
    {
        LogUtil::Debug(CODE_LOCATION, TEXT("Error in CloseHandle."));
        return;
    }
}

void MiscellaneousWinHook::MonitorRegChange_RegNotifyChangeValueMultiple()
{
    struct MonitorRegKeyInfo
    {
        HKEY monitorKey;
        HKEY mainKey;
        std::string subKey;
        HANDLE waitEvent;
        bool isRegistered;
    };

    MonitorRegKeyInfo allMonRegKeyInfos[] =
    {
        {NULL,HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", INVALID_HANDLE_VALUE,false},
        {NULL,HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall", INVALID_HANDLE_VALUE,false}
    };

    DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_ATTRIBUTES | REG_NOTIFY_CHANGE_LAST_SET | REG_NOTIFY_CHANGE_SECURITY;
    LONG   lErrorCode = ERROR_SUCCESS;

    int monitorKeyCount = sizeof(allMonRegKeyInfos) / sizeof(allMonRegKeyInfos[0]);
    for (int i = 0; i < monitorKeyCount; ++i)
    {
        // Open a key.
        if (allMonRegKeyInfos[i].monitorKey == NULL)
        {
            lErrorCode = RegOpenKeyEx(allMonRegKeyInfos[i].mainKey, allMonRegKeyInfos[i].subKey.c_str(), 0, KEY_NOTIFY, &allMonRegKeyInfos[i].monitorKey);
            if (lErrorCode != ERROR_SUCCESS)
            {
                LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegOpenKeyEx (%d)."), lErrorCode);
            }
        }

        // Create an event.
        if (allMonRegKeyInfos[i].waitEvent == INVALID_HANDLE_VALUE)
        {
            HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            if (hEvent == NULL)
            {
                LogUtil::Debug(CODE_LOCATION, TEXT("Error in CreateEvent (%d)."), GetLastError());
                return;
            }
            else
            {
                allMonRegKeyInfos[i].waitEvent = hEvent;
            }
        }
    }

    do
    {
        std::vector<HANDLE> waitEnvents;
        for (int i = 0; i < monitorKeyCount; ++i)
        {
            // Watch the registry key for a change of value.
            if (!allMonRegKeyInfos[i].isRegistered)
            {
                lErrorCode = RegNotifyChangeKeyValue(allMonRegKeyInfos[i].monitorKey, TRUE, dwFilter, allMonRegKeyInfos[i].waitEvent, TRUE);
                if (lErrorCode != ERROR_SUCCESS)
                {
                    LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegNotifyChangeKeyValue (%d)."), lErrorCode);
                    return;
                }
                else
                {
                    allMonRegKeyInfos[i].isRegistered = true;
                    if (allMonRegKeyInfos[i].waitEvent)
                    {
                        waitEnvents.push_back(allMonRegKeyInfos[i].waitEvent);
                    }
                }
            }
            else
            {
                waitEnvents.push_back(allMonRegKeyInfos[i].waitEvent);
            }
        }

        // Wait for an event to occur.
        DWORD evtCount = waitEnvents.size();
        const HANDLE* pAllEvents = waitEnvents.data();
        LogUtil::Debug(CODE_LOCATION, TEXT("Waiting for a change in the specified key..."));
        DWORD waitResult = WaitForMultipleObjects(evtCount, pAllEvents, FALSE, 1000);
        if (waitResult == WAIT_TIMEOUT)
        {
            LogUtil::Debug(CODE_LOCATION, TEXT("Wait time out"));
        }
        else
        {
            int evtIndex = waitResult - WAIT_OBJECT_0;
            if (0 <= evtIndex && evtIndex < evtCount)
            {
                std::string occurSubkey = "unknown";
                waitEnvents.clear();
                for (int i = 0; i < monitorKeyCount; ++i)
                {
                    if (allMonRegKeyInfos[i].waitEvent == *(pAllEvents + evtIndex))
                    {
                        occurSubkey = allMonRegKeyInfos[i].subKey;
                        allMonRegKeyInfos[i].isRegistered = false;
                    }
                    else if (allMonRegKeyInfos[i].waitEvent != INVALID_HANDLE_VALUE)
                    {
                        waitEnvents.push_back(allMonRegKeyInfos[i].waitEvent);
                    }
                }
                LogUtil::Debug(CODE_LOCATION, TEXT("Change has occurred. subkey=%s"), occurSubkey.c_str());
            }
            else
            {
                LogUtil::Error(CODE_LOCATION, TEXT("wait result a unknown handle"));
            }
        }
    } while (mIsMonitorRegRunning);

    for (int i = 0; i < monitorKeyCount; ++i)
    {
        // Close the key.
        lErrorCode = RegCloseKey(allMonRegKeyInfos[i].monitorKey);
        if (lErrorCode != ERROR_SUCCESS)
        {
            LogUtil::Debug(CODE_LOCATION, TEXT("Error in RegCloseKey (%d)."), GetLastError());
        }
        else
        {
            allMonRegKeyInfos[i].monitorKey = NULL;
        }

        // Close the handle.
        if (!CloseHandle(allMonRegKeyInfos[i].waitEvent))
        {
            LogUtil::Debug(CODE_LOCATION, TEXT("Error in CloseHandle."));
        }
        else
        {
            allMonRegKeyInfos[i].waitEvent = INVALID_HANDLE_VALUE;
        }
    }
}
