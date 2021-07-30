#ifndef MISCELLANEOUS_TIME_CODE_H
#define MISCELLANEOUS_TIME_CODE_H

#include <Windows.h>
#include <Mmddk.h>
#include <QList>

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousTimeCode;};

class QTimer;

//struct MidiDeviceInfo
//{
//    UINT    DevID;
//    QString DevName;
//};

class MiscellaneousTimeCode : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousTimeCode(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousTimeCode();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void InitUI();
    bool MidiEnumerateDevices();
    bool MidiDevicesCloseIn();
    bool MidiDevicesCloseOut();
    bool MidiDevicesOpenIn(UINT deviceID);
    bool MidiDevicesOpenOut(UINT deviceID);

    std::string MidiTechnologyToString(WORD wTechnology);
    std::string MidiSupportToString(DWORD dwSupport);
    std::string MidiMsgTypeToString(MMRESULT errorCode);
    std::string MidiErrorCodeToString(MMRESULT errorCode);
    static void CALLBACK MidiInProcedure(HMIDIIN hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static void CALLBACK MidiOutProcedure(HMIDIOUT hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

private slots:
    void TimeCodeEmiter_TimeOut();

    void on_cbMidiDevicesIn_currentIndexChanged(int index);
    void on_cbMidiDevicesOut_currentIndexChanged(int index);
    void on_btnTransferTest_clicked();
    void on_btnEventMapTest_clicked();
    void on_btnTimeEmiterTest_clicked();
    void on_btnEnumerateMIDI_clicked();
    void on_btnMtcStart_clicked();
    void on_btnMtcPause_clicked();
    void on_btnMtcStop_clicked();

private:
    Ui::MiscellaneousTimeCode       *ui;
    QTimer                          *mpTimeCodeEmiter;
    bool                             mbTimeCodeEnable;
    QString                          mBtnTimeEmiterText;
    long long                        mCurFrequency;
    QList<QPair<UINT, QString>>      mMidiDevPairIn;
    QList<QPair<UINT, QString>>      mMidiDevPairOut;
    HMIDIIN                          mHandleMidiIn;
    HMIDIOUT                         mHandleMidiOut;
};

#endif // MISCELLANEOUS_TIME_CODE_H
