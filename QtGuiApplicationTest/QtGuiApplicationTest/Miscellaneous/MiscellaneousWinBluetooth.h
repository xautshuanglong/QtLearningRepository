#ifndef MISCELLANEOUS_WIN_BLUETOOTH_H
#define MISCELLANEOUS_WIN_BLUETOOTH_H

#include "MiscellaneousBase.h"

#include <windows.h>
#include <string>
#include <thread>

#include "JCB_Logger/LogUtil.h"

namespace Ui { class MiscellaneousWinBluetooth; };

class MiscellaneousWinBluetooth : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinBluetooth(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinBluetooth();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnAllRadioDevice_clicked();
    void on_btnRemoteDevice_clicked();
    void on_btnRegisterCallbacks_clicked();
    void on_btnEmptyTest_clicked();

private:
    std::string GetRadioMacStr(PBYTE macAddress, size_t macLen);
    void PrintBthDeviceInfo(HANDLE hRadio);
    void PrintBthRadioInfo(HANDLE hRadio);
    void PrintBluetoothError(const CodeLocation &errorLocation, const std::string &errorFunc, DWORD errorCode);

private:
    Ui::MiscellaneousWinBluetooth *ui;
    bool mIsMonitorRegRunning;
    std::thread* mpRegMonitorThread;
};

#endif // MISCELLANEOUS_WIN_BLUETOOTH_H
