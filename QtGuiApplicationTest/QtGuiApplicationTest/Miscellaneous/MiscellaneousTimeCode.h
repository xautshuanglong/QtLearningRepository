#ifndef MISCELLANEOUS_TIME_CODE_H
#define MISCELLANEOUS_TIME_CODE_H

#include <Windows.h>
#include <Mmddk.h>
#include <QList>

#include "MiscellaneousBase.h"
#include "Core/TimeCodeObj.h"

namespace Ui {class MiscellaneousTimeCode;};

class QTimer;
class QSpinBox;

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
    TimeCodeObj GetTimeCodeFromUI(QSpinBox* pSpbHour, QSpinBox* pSpbMinute, QSpinBox* pSpbSecond, QSpinBox* pSpbFrame);

    std::string MidiTechnologyToString(WORD wTechnology);
    std::string MidiSupportToString(DWORD dwSupport);
    std::string MidiMsgTypeToString(MMRESULT errorCode);
    std::string MidiErrorCodeToString(MMRESULT errorCode);
    static void CALLBACK MidiInProcedure(HMIDIIN hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static void CALLBACK MidiOutProcedure(HMIDIOUT hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

private slots:
    void TimeCodeEmiter_TimeOut();
    void SlotTimeCodeChanged(const TimeCodeObj timecode);
    // Common Button
    void on_btnTransferTest_clicked();
    void on_btnEventMapTest_clicked();
    void on_btnTimeEmiterTest_clicked();
    void on_btnEnumerateMIDI_clicked();
    // Audio Input Output (LTC)
    void on_btnLtcStartStop_clicked();
    // MIDI InputOutput (MTC)
    void on_cbMidiDevicesIn_currentIndexChanged(int index);
    void on_cbMidiDevicesOut_currentIndexChanged(int index);
    void on_btnMtcStartStop_clicked();
    void on_btnMtcLocate_clicked();

signals:
    void SignalTimeCodeChanged(const TimeCodeObj timecode);

private:
    Ui::MiscellaneousTimeCode       *ui;
    QTimer                          *mpTimeCodeEmiter;
    int                              mMtcByteIndex;
    bool                             mbTimeCodeEnable; // ·¢ÉúÆ÷
    bool                             mbTimeCodeStarted;
    bool                             mbTimeCodeInputOn;
    QString                          mBtnTimeEmiterText;
    long long                        mCurFrequency;
    QList<QPair<UINT, QString>>      mMidiDevPairIn;
    QList<QPair<UINT, QString>>      mMidiDevPairOut;
    HMIDIIN                          mHandleMidiIn;
    HMIDIOUT                         mHandleMidiOut;
};

#endif // MISCELLANEOUS_TIME_CODE_H
