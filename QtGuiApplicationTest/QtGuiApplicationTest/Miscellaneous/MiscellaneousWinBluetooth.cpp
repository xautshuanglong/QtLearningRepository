#include "MiscellaneousWinBluetooth.h"
#include "ui_MiscellaneousWinBluetooth.h"

#include "Utils/StringUtil.h"
#include "Utils/TimeUtil.h"

#include <tchar.h>
#include <bluetoothapis.h>

using namespace Shuanglong::Utils;

MiscellaneousWinBluetooth::MiscellaneousWinBluetooth(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousWinBluetooth())
    , mIsMonitorRegRunning(false)
    , mpRegMonitorThread(nullptr)
{
    ui->setupUi(this);
}

MiscellaneousWinBluetooth::~MiscellaneousWinBluetooth()
{
    delete ui;
}

QString MiscellaneousWinBluetooth::GetTitle()
{
    return QObject::tr("Bluetooth");
}

QString MiscellaneousWinBluetooth::GetTitleTooltip()
{
    return QObject::tr("Using windows API to operate blue tooth device.");
}

MiscellaneousTestGroup MiscellaneousWinBluetooth::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinBluetooth::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Bluetooth;
}

void MiscellaneousWinBluetooth::on_btnAllRadioDevice_clicked()
{
    HANDLE hRadio = nullptr;
    BLUETOOTH_FIND_RADIO_PARAMS bthFindRadioParam = { 0 };
    bthFindRadioParam.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);

    HBLUETOOTH_RADIO_FIND hBthRadioFind = BluetoothFindFirstRadio(&bthFindRadioParam, &hRadio);
    if (hBthRadioFind == NULL)
    {
        this->PrintBluetoothError(CODE_LOCATION, "BluetoothFindFirstRadio", GetLastError());
        return;
    }
    do
    {
        PrintBthDeviceInfo(hRadio);
        PrintBthRadioInfo(hRadio);
        BOOL isConnectable = BluetoothIsConnectable(hRadio);
        BOOL isDiscoverable = BluetoothIsDiscoverable(hRadio);
        LogUtil::Info(CODE_LOCATION, "IsConnectable = %s", isConnectable ? "TRUE" : "FALSE");
        LogUtil::Info(CODE_LOCATION, "IsDiscoverable = %s", isDiscoverable ? "TRUE" : "FALSE");
    } while (BluetoothFindNextRadio(hBthRadioFind, &hRadio));
    BluetoothFindRadioClose(hBthRadioFind);
}

void MiscellaneousWinBluetooth::on_btnRemoteDevice_clicked()
{
    HANDLE hRadio = nullptr;
    BLUETOOTH_FIND_RADIO_PARAMS bthFindRadioParam = { 0 };
    bthFindRadioParam.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);

    HBLUETOOTH_RADIO_FIND hBthRadioFind = BluetoothFindFirstRadio(&bthFindRadioParam, &hRadio);
    if (hBthRadioFind == NULL)
    {
        this->PrintBluetoothError(CODE_LOCATION, "BluetoothFindFirstRadio", GetLastError());
        return;
    }
    do
    {
        BLUETOOTH_DEVICE_INFO bthDeviceInfo = { 0 };
        bthDeviceInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
        BLUETOOTH_DEVICE_SEARCH_PARAMS bthDeviceSearchParam = { 0 };
        bthDeviceSearchParam.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
        bthDeviceSearchParam.fReturnAuthenticated = TRUE;
        bthDeviceSearchParam.fReturnRemembered = TRUE;
        bthDeviceSearchParam.fReturnUnknown = TRUE;
        bthDeviceSearchParam.fReturnConnected = TRUE;
        bthDeviceSearchParam.fIssueInquiry = TRUE;
        bthDeviceSearchParam.cTimeoutMultiplier = 5;
        bthDeviceSearchParam.hRadio = hRadio;

        HBLUETOOTH_DEVICE_FIND hBthDeviceFind = BluetoothFindFirstDevice(&bthDeviceSearchParam, &bthDeviceInfo);
        if (hBthDeviceFind == NULL)
        {
            this->PrintBluetoothError(CODE_LOCATION, "BluetoothFindFirstDevice", GetLastError());
            return;
        }
        do
        {
            // Print Radio Information
            BOOL isConnectable = BluetoothIsConnectable(hRadio);
            BOOL isDiscoverable = BluetoothIsDiscoverable(hRadio);
            LogUtil::Info(CODE_LOCATION, "IsConnectable = %s", isConnectable ? "TRUE" : "FALSE");
            LogUtil::Info(CODE_LOCATION, "IsDiscoverable = %s", isDiscoverable ? "TRUE" : "FALSE");

            // Print Remote Device Information
            std::string radioNameStr = StringUtil::WStringToString(std::wstring(bthDeviceInfo.szName));
            std::string radioMacStr = this->GetRadioMacStr(bthDeviceInfo.Address.rgBytes, sizeof(bthDeviceInfo.Address.rgBytes));
            std::string lastSeenTimeStr = TimeUtil::GetDateTimeString(bthDeviceInfo.stLastSeen);
            std::string lastUsedTimeStr = TimeUtil::GetDateTimeString(bthDeviceInfo.stLastUsed);
            LogUtil::Info(CODE_LOCATION, "Device Name          : %s", radioNameStr.c_str());
            LogUtil::Info(CODE_LOCATION, "Device MAC           : 0x%08X  %s", bthDeviceInfo.Address.ullLong, radioMacStr.c_str());
            LogUtil::Info(CODE_LOCATION, "Device ClassOfDevice : 0x%08X", bthDeviceInfo.ulClassofDevice);
            LogUtil::Info(CODE_LOCATION, "Device Connected     : %s", bthDeviceInfo.fConnected ? "TRUE" : "FALSE");
            LogUtil::Info(CODE_LOCATION, "Device Remembered    : %s", bthDeviceInfo.fRemembered ? "TRUE" : "FALSE");
            LogUtil::Info(CODE_LOCATION, "Device Authenticated : %s", bthDeviceInfo.fAuthenticated ? "TRUE" : "FALSE");
            LogUtil::Info(CODE_LOCATION, "Device LastSeen      : %s", lastSeenTimeStr.c_str());
            LogUtil::Info(CODE_LOCATION, "Device LastUsed      : %s", lastUsedTimeStr.c_str());

        } while (BluetoothFindNextDevice(hBthDeviceFind, &bthDeviceInfo));
        BluetoothFindDeviceClose(hBthDeviceFind);
    } while (BluetoothFindNextRadio(hBthRadioFind, &hRadio));
    BluetoothFindRadioClose(hBthRadioFind);
}

void MiscellaneousWinBluetooth::on_btnRegisterCallbacks_clicked()
{
    int i = 0;
}

void MiscellaneousWinBluetooth::on_btnEmptyTest_clicked()
{
    int i = 0;
}

std::string MiscellaneousWinBluetooth::GetRadioMacStr(PBYTE macAddress, size_t macLen)
{
    std::string retValue;
    if (macAddress == nullptr || macLen == 0) {
        return retValue;
    }
    std::stringstream sstr;
    for (int i = 0; i < macLen; ++i) {
        if (sstr.rdbuf()->in_avail() > 0) {
            sstr << ":";
        }
        sstr << std::setw(2) << std::setfill('0') << std::right << std::hex << std::uppercase << (unsigned short)macAddress[macLen - i - 1];
    }
    retValue = sstr.str();
    return retValue;
}

void MiscellaneousWinBluetooth::PrintBthDeviceInfo(HANDLE hRadio)
{
    if (hRadio == nullptr) {
        return;
    }
    BLUETOOTH_DEVICE_INFO bthDeviceInfo = { 0 };
    bthDeviceInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
    DWORD errorCode = BluetoothGetDeviceInfo(hRadio, &bthDeviceInfo);
    if (errorCode != ERROR_SUCCESS) {
        this->PrintBluetoothError(CODE_LOCATION, "BluetoothGetDeviceInfo", errorCode);
        return;
    }
    std::string radioNameStr = StringUtil::WStringToString(std::wstring(bthDeviceInfo.szName));
    std::string radioMacStr = this->GetRadioMacStr(bthDeviceInfo.Address.rgBytes, sizeof(bthDeviceInfo.Address.rgBytes));
    std::string lastSeenTimeStr = TimeUtil::GetDateTimeString(bthDeviceInfo.stLastSeen);
    std::string lastUsedTimeStr = TimeUtil::GetDateTimeString(bthDeviceInfo.stLastUsed);
    LogUtil::Info(CODE_LOCATION, "Device Name          : %s", radioNameStr.c_str());
    LogUtil::Info(CODE_LOCATION, "Device MAC           : 0x%08X  %s", bthDeviceInfo.Address.ullLong, radioMacStr.c_str());
    LogUtil::Info(CODE_LOCATION, "Device ClassOfDevice : 0x%08X", bthDeviceInfo.ulClassofDevice);
    LogUtil::Info(CODE_LOCATION, "Device Connected     : %s", bthDeviceInfo.fConnected ? "TRUE" : "FALSE");
    LogUtil::Info(CODE_LOCATION, "Device Remembered    : %s", bthDeviceInfo.fRemembered ? "TRUE" : "FALSE");
    LogUtil::Info(CODE_LOCATION, "Device Authenticated : %s", bthDeviceInfo.fAuthenticated ? "TRUE" : "FALSE");
    LogUtil::Info(CODE_LOCATION, "Device LastSeen      : %s", lastSeenTimeStr.c_str());
    LogUtil::Info(CODE_LOCATION, "Device LastUsed      : %s", lastUsedTimeStr.c_str());
}

void MiscellaneousWinBluetooth::PrintBthRadioInfo(HANDLE hRadio)
{
    if (hRadio == nullptr) {
        return;
    }
    BLUETOOTH_RADIO_INFO bthRadioInfo = { 0 };
    bthRadioInfo.dwSize = sizeof(BLUETOOTH_RADIO_INFO);
    DWORD errorCode = BluetoothGetRadioInfo(hRadio, &bthRadioInfo);
    if (errorCode != ERROR_SUCCESS) {
        this->PrintBluetoothError(CODE_LOCATION, "BluetoothGetRadioInfo", errorCode);
        return;
    }
    std::string radioNameStr = StringUtil::WStringToString(std::wstring(bthRadioInfo.szName));
    std::string radioMacStr = this->GetRadioMacStr(bthRadioInfo.address.rgBytes, sizeof(bthRadioInfo.address.rgBytes));
    LogUtil::Info(CODE_LOCATION, "Radio Name          : %s", radioNameStr.c_str());
    LogUtil::Info(CODE_LOCATION, "Radio MAC           : 0x%08X  %s", bthRadioInfo.address.ullLong, radioMacStr.c_str());
    LogUtil::Info(CODE_LOCATION, "Radio ClassOfDevice : 0x%08X", bthRadioInfo.ulClassofDevice);
    LogUtil::Info(CODE_LOCATION, "Radio Subversion    : 0x%08X", bthRadioInfo.lmpSubversion);
    LogUtil::Info(CODE_LOCATION, "Radio Manufacturer  : 0x%08X", bthRadioInfo.manufacturer);
}

void MiscellaneousWinBluetooth::PrintBluetoothError(const CodeLocation &errorLocation, const std::string &errorFunc, DWORD errorCode)
{
    switch (errorCode)
    {
        case ERROR_NO_MORE_ITEMS:
            LogUtil::Warn(errorLocation, "Call %s failed! ErrorCode : ERROR_NO_MORE_ITEMS  0x%08X (%u)", errorFunc.c_str(), errorCode, errorCode);
            break;
        case ERROR_INVALID_PARAMETER:
            LogUtil::Warn(errorLocation, "Call %s failed! ErrorCode : ERROR_INVALID_PARAMETER  0x%08X (%u)", errorFunc.c_str(), errorCode, errorCode);
            break;
        case ERROR_REVISION_MISMATCH:
            LogUtil::Warn(errorLocation, "Call %s failed! ErrorCode : ERROR_REVISION_MISMATCH  0x%08X (%u)", errorFunc.c_str(), errorCode, errorCode);
            break;
        case ERROR_NOT_FOUND:
            LogUtil::Warn(errorLocation, "Call %s failed! ErrorCode : ERROR_NOT_FOUND  0x%08X (%u)", errorFunc.c_str(), errorCode, errorCode);
            break;
        case ERROR_OUTOFMEMORY:
            LogUtil::Warn(errorLocation, "Call %s failed! ErrorCode : ERROR_OUTOFMEMORY  0x%08X (%u)", errorFunc.c_str(), errorCode, errorCode);
            break;
        default:
            LogUtil::Error(errorLocation, "Call %s failed! ErrorCode : 0x%08X  (%u)", errorFunc.c_str(), errorCode, errorCode);
            break;
    }
}
