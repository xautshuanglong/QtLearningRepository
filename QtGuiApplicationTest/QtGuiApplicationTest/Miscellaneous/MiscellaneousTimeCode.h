#ifndef MISCELLANEOUS_TIME_CODE_H
#define MISCELLANEOUS_TIME_CODE_H

#include <Windows.h>
#include <Mmddk.h>
#include <QList>
#include <QHash>
#include <QFile>
#include <QAudioDeviceInfo>

#include "MiscellaneousBase.h"
#include "Core/TimeCodeObj.h"

namespace Ui {class MiscellaneousTimeCode;};

struct LTCDecoder;

class QTimer;
class QSpinBox;
class QComboBox;
class QAudioInput;
class QAudioOutput;
class QStyledItemDelegate;

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
    void ChangeComboxStyle(QComboBox* pCombox, QStyledItemDelegate* pStyleDelegate);
    void UpdateComboxAudio(QComboBox* pCombox, const QHash<QString, QAudioDeviceInfo> &audioDeviceInfoHash);
    void UpdateComboxMidi();
    void CloseFile(QFile& file);
    void OpenFile(QFile& file);
    void AudioEnumerateDevices(QHash<QString, QAudioDeviceInfo> &audioDeviceHash, QAudio::Mode audioMode);
    bool MidiEnumerateDevices();
    bool MidiDevicesCloseIn();
    bool MidiDevicesCloseOut();
    bool MidiDevicesOpenIn(UINT deviceID);
    bool MidiDevicesOpenOut(UINT deviceID);
    TimeCodeObj GetTimeCodeFromUI(QSpinBox* pSpbHour, QSpinBox* pSpbMinute, QSpinBox* pSpbSecond, QSpinBox* pSpbFrame);

    std::string AudioErrorToString(QAudio::Error error);
    std::string MidiTechnologyToString(WORD wTechnology);
    std::string MidiSupportToString(DWORD dwSupport);
    std::string MidiMsgTypeToString(MMRESULT errorCode);
    std::string MidiErrorCodeToString(MMRESULT errorCode);
    static void CALLBACK MidiInProcedure(HMIDIIN hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static void CALLBACK MidiOutProcedure(HMIDIOUT hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

private slots:
    void TimeCodeEmiter_TimeOut();
    void SlotTimeCodeMtcChanged(const TimeCodeObj timecode);
    void SlotTimeCodeLtcChanged(const TimeCodeObj timecode);
    void SlotAudioInputDestroyed(QObject *pObj);
    void SlotAudioInputStateChanged(QAudio::State state);
    // QAudioInput IO
    void SlotAudioIoDestroyed(QObject *pObj = Q_NULLPTR);
    void SlotAudioIoAboutToClose();
    void SlotAudioIoBytesWritten(qint64 bytes);
    void SlotAudioIoReadyRead();
    void SlotAudioIoChannelBytesWritten(int channel, qint64 bytes);
    void SlotAudioIoChannelReadyRead(int channel);
    void SlotAudioIoReadChannelFinished();
    // Common Button
    void on_btnTransferTest_clicked();
    void on_btnEventMapTest_clicked();
    void on_btnTimeEmiterTest_clicked();
    void on_btnEnumerateDevices_clicked();
    // Audio Input Output (Clock)
    void on_cbAudioDevicesClockIn_currentIndexChanged(int index);
    void on_cbAudioDevicesClockOut_currentIndexChanged(int index);
    void on_btnClockStartStop_clicked();
    // Audio Input Output (LTC)
    void on_cbAudioDevicesLtcIn_currentIndexChanged(int index);
    void on_cbAudioDevicesLtcOut_currentIndexChanged(int index);
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
    bool                             mbTimeCodeEnable;    // 模拟时间码发生器使能
    bool                             mbTimeCodeStarted;   // 启动 或 停止时间码发生设备（USB）
    bool                             mbTimeCodeInputOn;   // 标记时间码设备读取功能是否已打开（USB），设备改变后设为 FALSE，需重新开启
    bool                             mbAudioInputStarted; // 标记音频设备读取功能是否已打开
    bool                             mbAudioInputChanged; // 选择其他音频输入设备
    bool                             mbAudioOutputChanged;// 选择其他音频输出设备
    QString                          mBtnTimeEmiterText;  // 模拟时间码发生器按钮显示文本
    long long                        mCurFrequency;       // 高性能计数器工作频率
    long long                        mLtcStreamOffset;    // LTC 音频流位置信息，全局累加
    QList<QPair<UINT, QString>>      mMidiDevPairIn;      // 可用 MIDI 输入设备
    QList<QPair<UINT, QString>>      mMidiDevPairOut;     // 可用 MIDI 输出设备
    QHash<QString, QAudioDeviceInfo> mHashAudioInput;
    QHash<QString, QAudioDeviceInfo> mHashAudioOutput;
    HMIDIIN                          mHandleMidiIn;
    HMIDIOUT                         mHandleMidiOut;
    QAudioDeviceInfo                 mAudioDeviceInfoIn;
    QAudioDeviceInfo                 mAudioDeviceInfoOut;
    QAudioInput                     *mpAudioInput;
    QAudioOutput                    *mpAudioOutput;
    QIODevice                       *mpAudioDeviceIO;
    QFile                            mpFilePcmAudioIn;
    LTCDecoder                      *mpLtcDecoder;
};

#endif // MISCELLANEOUS_TIME_CODE_H
