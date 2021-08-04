#include "MiscellaneousTimeCode.h"
#include "ui_MiscellaneousTimeCode.h"

#include <QTimer>
#include <QDateTime>
#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QAudioDeviceInfo>

#include "Utils/TimeUtil.h"
#include "LogUtil.h"

using namespace Shuanglong::Utils;

MiscellaneousTimeCode::MiscellaneousTimeCode(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousTimeCode())
    , mMtcByteIndex(0)
    , mbTimeCodeEnable(false)
    , mbTimeCodeStarted(false)
    , mbTimeCodeInputOn(false)
    , mHandleMidiIn(NULL)
    , mHandleMidiOut(NULL)
{
    ui->setupUi(this);

    this->MidiEnumerateDevices();
    this->InitUI();

    // 高性能计数器
    mCurFrequency = TimeUtil::QueryPerformanceFrequency();

    this->connect(this, SIGNAL(SignalTimeCodeChanged(const TimeCodeObj)), this, SLOT(SlotTimeCodeChanged(const TimeCodeObj)));
}

MiscellaneousTimeCode::~MiscellaneousTimeCode()
{
    this->MidiDevicesCloseOut();
    this->MidiDevicesCloseIn();
    delete ui;
}

QString MiscellaneousTimeCode::GetTitle()
{
    return QObject::tr("Time Code");
}

QString MiscellaneousTimeCode::GetTitleTooltip()
{
    return QObject::tr("How to us time code MTC/LTC.");
}

MiscellaneousTestGroup MiscellaneousTimeCode::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousTimeCode::GetItemID()
{
    return MiscellaneousTestItem::Others_TimeCode;
}

void MiscellaneousTimeCode::InitUI()
{
    // 下拉组合框样式
    QStyledItemDelegate* pItemDelegate = new QStyledItemDelegate();
    ui->cbAudioDevicesIn->setItemDelegate(pItemDelegate);
    ui->cbAudioDevicesIn->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->cbAudioDevicesIn->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    ui->cbAudioDevicesOut->setItemDelegate(pItemDelegate);
    ui->cbAudioDevicesOut->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->cbAudioDevicesOut->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    ui->cbMidiDevicesIn->setItemDelegate(pItemDelegate);
    ui->cbMidiDevicesIn->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->cbMidiDevicesIn->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    ui->cbMidiDevicesOut->setItemDelegate(pItemDelegate);
    ui->cbMidiDevicesOut->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->cbMidiDevicesOut->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    // UI 控件初始化
    mBtnTimeEmiterText = ui->btnTimeEmiterTest->text();
    ui->btnTimeEmiterTest->setText(QString("%1 (disable)").arg(mBtnTimeEmiterText));

    // 时间码触发器
    mpTimeCodeEmiter = new QTimer(this);
    mpTimeCodeEmiter->setInterval(std::chrono::milliseconds(1));
    mpTimeCodeEmiter->setTimerType(Qt::PreciseTimer);
    Qt::TimerType timerType = mpTimeCodeEmiter->timerType();
    connect(mpTimeCodeEmiter, SIGNAL(timeout()), this, SLOT(TimeCodeEmiter_TimeOut()));

    // MIDI 设备列表选项
    ui->cbMidiDevicesIn->clear();
    for (int i = 0; i < mMidiDevPairIn.size(); ++i)
    {
        ui->cbMidiDevicesIn->addItem(mMidiDevPairIn[i].second, QVariant::fromValue(mMidiDevPairIn[i].first));
    }
    ui->cbMidiDevicesOut->clear();
    for (int i = 0; i < mMidiDevPairOut.size(); ++i)
    {
        ui->cbMidiDevicesOut->addItem(mMidiDevPairOut[i].second, QVariant::fromValue(mMidiDevPairOut[i].first));
    }
}

bool MiscellaneousTimeCode::MidiEnumerateDevices()
{
    bool retValue = true;

    /*
    Header file Mmddk.h defines the following system-intercepted device messages:
    DRV_QUERYDEVICEINTERFACE          For more information, see Obtaining a Device Interface Name.
    DRV_QUERYDEVICEINTERFACESIZE      For more information, see Obtaining a Device Interface Name.
    DRV_QUERYDEVNODE                  Queries for a device's devnode number.
    DRV_QUERYMAPPABLE                 Queries whether a device can be used by a mapper.
    DRVM_MAPPER_CONSOLEVOICECOM_GET   For more information, see Preferred Voice-Communications Device ID.
    DRVM_MAPPER_PREFERRED_GET         For more information, see Accessing the Preferred Device ID.
    */
    mMidiDevPairIn.clear();
    UINT midiInNum = midiInGetNumDevs();
    for (UINT i = 0; i < midiInNum; ++i)
    {
        MIDIINCAPS midiCaps;
        midiInGetDevCaps(i, &midiCaps, sizeof(MIDIINCAPS));
        mMidiDevPairIn.push_back(qMakePair(i, QString::fromLocal8Bit(midiCaps.szPname)));
        // 输出 MIDI 输入设备信息
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : MID=%u PID=%u", midiCaps.wMid, midiCaps.wPid);
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : DRIVER_VERSION=%u.%u", HIBYTE(midiCaps.vDriverVersion), LOBYTE(midiCaps.vDriverVersion));
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : NAME=%s", midiCaps.szPname);
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : SUPPORT=%s", this->MidiSupportToString(midiCaps.dwSupport).c_str());
    }

    mMidiDevPairOut.clear();
    UINT midiOutNum = midiOutGetNumDevs();
    for (UINT i = 0; i < midiOutNum; ++i)
    {
        MIDIOUTCAPS midiCaps;
        midiOutGetDevCaps(i, &midiCaps, sizeof(MIDIOUTCAPS));
        mMidiDevPairOut.push_back(qMakePair(i, QString::fromLocal8Bit(midiCaps.szPname)));
        // 输出 MIDI 输出设备信息
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : MID=%u PID=%u", midiCaps.wMid, midiCaps.wPid);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : DRIVER_VERSION=%u.%u", HIBYTE(midiCaps.vDriverVersion), LOBYTE(midiCaps.vDriverVersion));
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : NAME=%s", midiCaps.szPname);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : TECHNOLOGY=%s", this->MidiTechnologyToString(midiCaps.wTechnology).c_str());
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : VOICES=%u", midiCaps.wVoices);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : MAX_NOTES=%u", midiCaps.wNotes);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : CHANNEL=%u", midiCaps.wChannelMask);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : SUPPORT=%s", this->MidiSupportToString(midiCaps.dwSupport).c_str());
    }

    return retValue;
}

bool MiscellaneousTimeCode::MidiDevicesCloseIn()
{
    bool retValue = true;
    if (mHandleMidiIn)
    {
        MMRESULT res = midiInClose(mHandleMidiIn);
        if (res == MMSYSERR_NOERROR)
        {
            mHandleMidiIn = NULL;
        }
        else
        {
            retValue = false;
            LogUtil::Debug(CODE_LOCATION, "midiInClose failed: %s", this->MidiErrorCodeToString(res).c_str());
        }
    }
    return retValue;
}

bool MiscellaneousTimeCode::MidiDevicesCloseOut()
{
    bool retValue = true;
    if (mHandleMidiOut)
    {
        MMRESULT res = midiOutClose(mHandleMidiOut);
        if (res == MMSYSERR_NOERROR)
        {
            mHandleMidiOut = NULL;
        }
        else
        {
            retValue = false;
            LogUtil::Debug(CODE_LOCATION, "midiOutClose failed: %s", this->MidiErrorCodeToString(res).c_str());
        }
    }
    return retValue;
}

bool MiscellaneousTimeCode::MidiDevicesOpenIn(UINT deviceID)
{
    bool retValue = this->MidiDevicesCloseIn();
    if (!retValue)
    {
        return retValue;
    }

    MMRESULT res = midiInOpen(&mHandleMidiIn, deviceID, (DWORD_PTR)MiscellaneousTimeCode::MidiInProcedure, (DWORD_PTR)this, CALLBACK_FUNCTION);
    if (res == MMSYSERR_NOERROR)
    {
        retValue = true;
    }
    else
    {
        retValue = false;
        LogUtil::Debug(CODE_LOCATION, "midiInOpen failed: %s", this->MidiErrorCodeToString(res).c_str());
    }

    return retValue;
}

bool MiscellaneousTimeCode::MidiDevicesOpenOut(UINT deviceID)
{
    bool retValue = this->MidiDevicesCloseOut();
    if (!retValue)
    {
        return retValue;
    }

    MMRESULT res = midiOutOpen(&mHandleMidiOut, deviceID, (DWORD_PTR)MiscellaneousTimeCode::MidiOutProcedure, (DWORD_PTR)this, CALLBACK_FUNCTION);
    if (res == MMSYSERR_NOERROR)
    {
        retValue = true;
    }
    else
    {
        retValue = false;
        LogUtil::Debug(CODE_LOCATION, "midiOutOpen failed: %s", this->MidiErrorCodeToString(res).c_str());
    }

    return retValue;
}

TimeCodeObj MiscellaneousTimeCode::GetTimeCodeFromUI(QSpinBox* pSpbHour, QSpinBox* pSpbMinute, QSpinBox* pSpbSecond, QSpinBox* pSpbFrame)
{
    int frameRate = 0;
    int hour = pSpbHour->value();
    int minute = pSpbMinute->value();
    int second = pSpbSecond->value();
    int frame = pSpbFrame->value();
    return TimeCodeObj(hour, minute, second, frame, frameRate);
}

std::string MiscellaneousTimeCode::MidiTechnologyToString(WORD wTechnology)
{
    std::string retValue = "UNKNOWN";
    switch (wTechnology)
    {
    case MOD_MIDIPORT: // MIDI hardware port.
        retValue = "MOD_MIDIPORT";
        break;
    case MOD_SYNTH: // Synthesizer.
        retValue = "MOD_SYNTH";
        break;
    case MOD_SQSYNTH: // Square wave synthesizer.
        retValue = "MOD_SQSYNTH";
        break;
    case MOD_FMSYNTH: // FM synthesizer.
        retValue = "MOD_FMSYNTH";
        break;
    case MOD_MAPPER: // Microsoft MIDI mapper.
        retValue = "MOD_MAPPER";
        break;
    case MOD_WAVETABLE: // Hardware wavetable synthesizer.
        retValue = "MOD_WAVETABLE";
        break;
    case MOD_SWSYNTH: // Software synthesizer.
        retValue = "MOD_SWSYNTH";
        break;
    default:
        break;
    }
    return retValue;
}

std::string MiscellaneousTimeCode::MidiSupportToString(DWORD dwSupport)
{
    std::string retValue = "UNKNOWN";
    switch (dwSupport)
    {
    case MIDICAPS_CACHE: // Supports patch caching.
        retValue = "MIDICAPS_CACHE";
        break;
    case MIDICAPS_LRVOLUME: // Supports separate leftand right volume control.
        retValue = "MIDICAPS_LRVOLUME";
        break;
    case MIDICAPS_STREAM: // Provides direct support for the midiStreamOut function.
        retValue = "MIDICAPS_STREAM";
        break;
    case MIDICAPS_VOLUME: // Supports volume control.
        retValue = "MIDICAPS_VOLUME";
        break;
    default:
        break;
    }
    return retValue;
}

std::string MiscellaneousTimeCode::MidiMsgTypeToString(MMRESULT errorCode)
{
    std::string retValue = "UNKNOWN";
    switch (errorCode)
    {
    case MIM_OPEN: // The MIM_OPEN message is sent to a MIDI input callback function when a MIDI input device is opened.
        retValue = "MOM_OPEN";
        break;
    case MIM_CLOSE: // The MIM_CLOSE message is sent to a MIDI input callback function when a MIDI input device is closed.
        retValue = "MIM_CLOSE";
        break;
    case MIM_DATA: // The MIM_DATA message is sent to a MIDI input callback function when a MIDI message is received by a MIDI input device.
        retValue = "MIM_DATA";
        break;
    case MIM_LONGDATA: // The MIM_LONGDATA message is sent to a MIDI input callback function when a system-exclusive buffer has been filled with data and is being returned to the application.
        retValue = "MIM_LONGDATA";
        break;
    case MIM_ERROR: // The MIM_ERROR message is sent to a MIDI input callback function when an invalid MIDI message is received.
        retValue = "MIM_ERROR";
        break;
    case MIM_LONGERROR: // The MIM_LONGERROR message is sent to a MIDI input callback function when an invalid or incomplete MIDI system-exclusive message is received.
        retValue = "MIM_LONGERROR";
        break;
    case MIM_MOREDATA:         // The MIM_MOREDATA message is sent to a MIDI input callback function
        retValue = "MIM_MOREDATA"; // when a MIDI message is received by a MIDI input device but the application is not processing MIM_DATA messages fast enough to keep up with the input device driver.
        break;                 //The callback function receives this message only when the application specifies MIDI_IO_STATUS in the call to the midiInOpen function.
    case MOM_OPEN: // The MOM_OPEN message is sent to a MIDI output callback function when a MIDI output device is opened.
        retValue = "MOM_OPEN";
        break;
    case MOM_CLOSE: // The MOM_CLOSE message is sent to a MIDI output callback function when a MIDI output device is closed.
        retValue = "MOM_CLOSE";
        break;
    case MOM_DONE: // The MOM_DONE message is sent to a MIDI output callback function when the specified system-exclusive or stream buffer has been played and is being returned to the application.
        retValue = "MOM_DONE";
        break;
    case MOM_POSITIONCB: // The MOM_POSITION message is sent when an MEVT_F_CALLBACK event is reached in the MIDI output stream.
        retValue = "MOM_POSITIONCB";
        break;
    default:
        break;
    }
    return retValue;
}

std::string MiscellaneousTimeCode::MidiErrorCodeToString(MMRESULT errorCode)
{
    std::string retValue = "UNKNOWN";
    switch (errorCode)
    {
    case MIDIERR_NODEVICE: // No MIDI port was found.This error occurs only when the mapper is opened.
        retValue = "MIDIERR_NODEVICE";
        break;
    case MMSYSERR_NOERROR:
        retValue = "MMSYSERR_NOERROR";
        break;
    case MMSYSERR_ERROR:
        retValue = "MMSYSERR_ERROR";
        break;
    case MMSYSERR_BADDEVICEID: // The specified device identifier is out of range.
        retValue = "MMSYSERR_BADDEVICEID";
        break;
    case MMSYSERR_NOTENABLED:
        retValue = "MMSYSERR_NOTENABLED";
        break;
    case MMSYSERR_ALLOCATED: // The specified resource is already allocated.
        retValue = "MMSYSERR_ALLOCATED";
        break;
    case MMSYSERR_INVALHANDLE:
        retValue = "MMSYSERR_INVALHANDLE";
        break;
    case MMSYSERR_NODRIVER:
        retValue = "MMSYSERR_NODRIVER";
        break;
    case MMSYSERR_NOMEM: // The system is unable to allocate or lock memory.
        retValue = "MMSYSERR_NOMEM";
        break;
    case MMSYSERR_NOTSUPPORTED:
        retValue = "MMSYSERR_NOTSUPPORTED";
        break;
    case MMSYSERR_BADERRNUM:
        retValue = "MMSYSERR_BADERRNUM";
        break;
    case MMSYSERR_INVALFLAG:
        retValue = "MMSYSERR_INVALFLAG";
        break;
    case MMSYSERR_INVALPARAM: /* invalid parameter passed */
        retValue = "MMSYSERR_INVALPARAM";
        break;
    case MMSYSERR_HANDLEBUSY: /* handle being used */
        retValue = "MMSYSERR_HANDLEBUSY";
        break;
    case MMSYSERR_INVALIDALIAS: /* specified alias not found */
        retValue = "MMSYSERR_INVALIDALIAS";
        break;
    case MMSYSERR_BADDB: /* bad registry database */
        retValue = "MMSYSERR_BADDB";
        break;
    case MMSYSERR_KEYNOTFOUND: /* registry key not found */
        retValue = "MMSYSERR_KEYNOTFOUND";
        break;
    case MMSYSERR_READERROR: /* registry read error */
        retValue = "MMSYSERR_READERROR";
        break;
    case MMSYSERR_WRITEERROR: /* registry write error */
        retValue = "MMSYSERR_WRITEERROR";
        break;
    case MMSYSERR_DELETEERROR: /* registry delete error */
        retValue = "MMSYSERR_DELETEERROR";
        break;
    case MMSYSERR_VALNOTFOUND: /* registry value not found */
        retValue = "MMSYSERR_VALNOTFOUND";
        break;
    case MMSYSERR_NODRIVERCB: /* driver does not call DriverCallback */
        retValue = "MMSYSERR_NODRIVERCB";
        break;
    case MMSYSERR_MOREDATA: /* more data to be returned */
        retValue = "MMSYSERR_MOREDATA";
        break;
    default:
        break;
    }
    return retValue;
}

void MiscellaneousTimeCode::MidiInProcedure(HMIDIIN hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    if (dwInstance == NULL) return;

    static int gRealtimeMtcHour = 0;
    static int gRealtimeMtcMinute = 0;
    static int gRealtimeMtcSecond = 0;
    static int gRealtimeMtcFrame = 0;

    MiscellaneousTimeCode* pInstance = static_cast<MiscellaneousTimeCode*>((void*)dwInstance);
    if (wMsg != MIM_DATA)
    {
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : MESSAGE --> %s", pInstance->MidiMsgTypeToString(wMsg).c_str());
    }

    switch (wMsg)
    {
    case MIM_OPEN:
        break;
    case MIM_CLOSE:
        break;
    case MIM_DATA:
    {
        BYTE *pMidiData = (BYTE *)&dwParam1;
        if (pMidiData[0] != 0xF1) break;

        BYTE mtcByte = pMidiData[1];
        int byteValue = mtcByte & 0x0F;
        int byteIndex = (mtcByte >> 4 & 0x0F);

        if (byteIndex == 0)      // 帧数 低位
        {
            gRealtimeMtcFrame = byteValue & 0x0F;
        }
        else if (byteIndex == 1) // 帧数 高位
        {
            gRealtimeMtcFrame = gRealtimeMtcFrame | (byteValue << 4);
        }
        else if (byteIndex == 2) // 秒钟 低位
        {
            gRealtimeMtcSecond = byteValue & 0x0F;
        }
        else if (byteIndex == 3) // 秒钟 高位
        {
            gRealtimeMtcSecond = gRealtimeMtcSecond | (byteValue << 4);
        }
        else if (byteIndex == 4) // 分钟 低位
        {
            gRealtimeMtcMinute = byteValue & 0x0F;
        }
        else if (byteIndex == 5) // 分钟 高位
        {
            gRealtimeMtcMinute = gRealtimeMtcMinute | (byteValue << 4);
        }
        else if (byteIndex == 6) // 小时 低位
        {
            gRealtimeMtcHour = byteValue & 0x0F;
        }
        else if (byteIndex == 7) // 小时 高位
        {
            gRealtimeMtcHour = gRealtimeMtcHour | ((byteValue & 0x01) << 4);
            int frameRate = 0;
            int frameType = byteValue >> 1 & 0x03;
            if (frameType == 0)
            {
                frameRate = 24;
            }
            else if (frameType == 1)
            {
                frameRate = 25;
            }
            else if (frameType == 2)
            {
                frameRate = 30; // drop frame
            }
            else if (frameType == 3)
            {
                frameRate = 30;
            }
            // 时间码结束
            emit pInstance->SignalTimeCodeChanged(TimeCodeObj(gRealtimeMtcHour, gRealtimeMtcMinute, gRealtimeMtcSecond, gRealtimeMtcFrame, frameRate));
        }
        //DWORD64 dwTimestamp = dwParam2;
        //QDateTime startTime = QDateTime::fromMSecsSinceEpoch(dwTimestamp, Qt::LocalTime);
        //QString startTimeStr = startTime.toString("yyyy-MM-dd hh:mm:ss");
        //LogUtil::Debug(CODE_LOCATION, "MessageData %02X %02X %02X %02X  TimeStamp %s",
        //    pMidiData[0], pMidiData[1], pMidiData[2], pMidiData[3], startTimeStr.toUtf8().data());
        break;
    }
    case MIM_LONGDATA:
        break;
    case MIM_ERROR:
        break;
    case MIM_LONGERROR:
        break;
    case MIM_MOREDATA:
        break;
    default:
        break;
    }
}

void MiscellaneousTimeCode::MidiOutProcedure(HMIDIOUT hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    if (dwInstance == NULL) return;

    MiscellaneousTimeCode* pInstance = static_cast<MiscellaneousTimeCode*>((void*)dwInstance);
    LogUtil::Debug(CODE_LOCATION, "MIDI OUT : MESSAGE --> %s", pInstance->MidiMsgTypeToString(wMsg).c_str());

    switch (wMsg)
    {
    case MOM_OPEN:
        break;
    case MOM_CLOSE:
        break;
    case MOM_DONE:
        break;
    case MOM_POSITIONCB:
        break;
    default:
        break;
    }
}

void MiscellaneousTimeCode::TimeCodeEmiter_TimeOut()
{
    //static LONGLONG oldTickCount = TimeUtil::QueryPerformanceCounter();
    //LONGLONG curTickCount = TimeUtil::QueryPerformanceCounter();
    //LONGLONG diffCount = curTickCount - oldTickCount;
    //oldTickCount = curTickCount;
    //double microSecond = diffCount * 1000000 / mCurFrequency;
    //LogUtil::Debug(CODE_LOCATION, "curFrequency: %lld    curTickCount: %lld    diff: %lld  ( %llf us )",
    //    mCurFrequency, curTickCount, diffCount, microSecond);

    // 60fpx    --> 166667 = 16.6667ms * 10^4 = 0.0166667s * 10^7
    // 30fpx    --> 333333 = 33.3333ms * 10^4 = 0.0333333s * 10^7
    // 29.97fpx --> 333667 = 33.3667ms * 10^4 = 0.0333667s * 10^7
    // 25fpx    --> 400000 = 40.0000ms * 10^4 = 0.0400000s * 10^7
    // 24fpx    --> 416667 = 41.6667ms * 10^4 = 0.0416667s * 10^7

    static LONGLONG oldTickCount = TimeUtil::QueryPerformanceCounter();
    LONGLONG curTickCount = TimeUtil::QueryPerformanceCounter();
    LONGLONG diffTickCount = curTickCount - oldTickCount;

    bool emitFlag = false;
    if (diffTickCount >= 166666)
    {
        emitFlag = true; // 超时触发
    }
    else if (diffTickCount > 160000)
    {
        while (diffTickCount < 166666)
        {
            curTickCount = TimeUtil::QueryPerformanceCounter();
            diffTickCount = curTickCount - oldTickCount;
        }
        emitFlag = true;
    }
    if (emitFlag)
    {
        oldTickCount = curTickCount;
        double microSecond = diffTickCount * 1000.0 / mCurFrequency;
        LogUtil::Debug(CODE_LOCATION, "curFrequency: %lld    diffTickCount: %lld    %llf ms",
            mCurFrequency, diffTickCount, microSecond);
    }
}

void MiscellaneousTimeCode::SlotTimeCodeChanged(const TimeCodeObj timecode)
{
    ui->lcdTimeCodeMTC->display(QString::fromStdString(std::to_string(timecode)));
    ui->lcdFrameRate->display(timecode.getFrameRate());
}

void MiscellaneousTimeCode::on_btnTransferTest_clicked()
{
    int i = 0;
}

void MiscellaneousTimeCode::on_btnEventMapTest_clicked()
{
    int i = 0;
}

void MiscellaneousTimeCode::on_btnTimeEmiterTest_clicked()
{
    if (mbTimeCodeEnable)
    {
        mpTimeCodeEmiter->stop();
    }
    else
    {
        mpTimeCodeEmiter->start();
    }
    mbTimeCodeEnable = !mbTimeCodeEnable;
    if (mbTimeCodeEnable)
    {
        ui->btnTimeEmiterTest->setText(QString("%1 (enable)").arg(mBtnTimeEmiterText));
    } 
    else
    {
        ui->btnTimeEmiterTest->setText(QString("%1 (disable)").arg(mBtnTimeEmiterText));
    }
}

void MiscellaneousTimeCode::on_btnEnumerateMIDI_clicked()
{
    this->MidiEnumerateDevices();
    return;

    /*
    Header file Mmddk.h defines the following system-intercepted device messages:
    DRV_QUERYDEVICEINTERFACE          For more information, see Obtaining a Device Interface Name.
    DRV_QUERYDEVICEINTERFACESIZE      For more information, see Obtaining a Device Interface Name.
    DRV_QUERYDEVNODE                  Queries for a device's devnode number.
    DRV_QUERYMAPPABLE                 Queries whether a device can be used by a mapper.
    DRVM_MAPPER_CONSOLEVOICECOM_GET   For more information, see Preferred Voice-Communications Device ID.
    DRVM_MAPPER_PREFERRED_GET         For more information, see Accessing the Preferred Device ID.
    */
    UINT midiInNum = midiInGetNumDevs();
    for (UINT i = 0; i < midiInNum; ++i)
    {
        MIDIINCAPS midiCaps;
        midiInGetDevCaps(i, &midiCaps, sizeof(MIDIINCAPS));
        mMidiDevPairIn.push_back(qMakePair(i, QString::fromLocal8Bit(midiCaps.szPname)));
        // 输出 MIDI 输入设备信息
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : MID=%u PID=%u", midiCaps.wMid, midiCaps.wPid);
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : DRIVER_VERSION=%u.%u", HIBYTE(midiCaps.vDriverVersion), LOBYTE(midiCaps.vDriverVersion));
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : NAME=%s", midiCaps.szPname);
        LogUtil::Debug(CODE_LOCATION, "MIDI IN : SUPPORT=%s", this->MidiSupportToString(midiCaps.dwSupport).c_str());
    }

    UINT midiOutNum = midiOutGetNumDevs();
    for (UINT i = 0; i < midiOutNum; ++i)
    {
        MIDIOUTCAPS midiCaps;
        midiOutGetDevCaps(i, &midiCaps, sizeof(MIDIOUTCAPS));
        mMidiDevPairOut.push_back(qMakePair(i, QString::fromLocal8Bit(midiCaps.szPname)));
        // 输出 MIDI 输出设备信息
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : MID=%u PID=%u", midiCaps.wMid, midiCaps.wPid);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : DRIVER_VERSION=%u.%u", HIBYTE(midiCaps.vDriverVersion), LOBYTE(midiCaps.vDriverVersion));
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : NAME=%s", midiCaps.szPname);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : TECHNOLOGY=%s", this->MidiTechnologyToString(midiCaps.wTechnology).c_str());
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : VOICES=%u", midiCaps.wVoices);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : MAX_NOTES=%u", midiCaps.wNotes);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : CHANNEL=%u", midiCaps.wChannelMask);
        LogUtil::Debug(CODE_LOCATION, "MIDI OUT : SUPPORT=%s", this->MidiSupportToString(midiCaps.dwSupport).c_str());

        if (strcmp(midiCaps.szPname, "loopMIDI Port") == 0)
        {
            HMIDIOUT hLoopMIDI = 0;
            MMRESULT res = midiOutOpen(&hLoopMIDI, i, (DWORD_PTR)MiscellaneousTimeCode::MidiOutProcedure, (DWORD_PTR)this, CALLBACK_FUNCTION);
            if (res == MMSYSERR_NOERROR)
            {
                // 打开成功 —— 写数据测试
                DWORD bufferSize = 0;
                MMRESULT outMsgRes = midiOutMessage(hLoopMIDI, DRV_QUERYDEVICEINTERFACESIZE, (DWORD_PTR)&bufferSize, 0);
                LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiOutMessage result DRV_QUERYDEVICEINTERFACESIZE --> %s  bufferSize=%lu",
                    this->MidiErrorCodeToString(outMsgRes).c_str(), bufferSize);
                TCHAR* pNameBuffer = new TCHAR[bufferSize];
                ZeroMemory(pNameBuffer, bufferSize);
                outMsgRes = midiOutMessage(hLoopMIDI, DRV_QUERYDEVICEINTERFACE, (DWORD_PTR)pNameBuffer, bufferSize - 1);
                LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiOutMessage result DRV_QUERYDEVICEINTERFACE --> %s interfaceName=%s",
                    this->MidiErrorCodeToString(outMsgRes).c_str(), pNameBuffer);
                delete[]pNameBuffer;

                MIDIHDR midiMsgBuffer;
                outMsgRes = midiOutLongMsg(hLoopMIDI, &midiMsgBuffer, sizeof(MIDIHDR));
                LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiOutLongMsg result --> %s",
                    this->MidiErrorCodeToString(outMsgRes).c_str());

                DWORD dwMessage = 0x01010101;
                outMsgRes = midiOutShortMsg(hLoopMIDI, dwMessage);
                LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiOutShortMsg result --> %s",
                    this->MidiErrorCodeToString(outMsgRes).c_str());
            }
            else
            {
                // 打开失败 —— 输出错误码
                LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiOutOpen failed --> %s", this->MidiErrorCodeToString(res).c_str());
            }
            midiOutClose(hLoopMIDI);
        }
    }
}

void MiscellaneousTimeCode::on_btnLtcStartStop_clicked()
{
    ui->lcdTimeCodeLTC->display(QTime::currentTime().toString("hh:mm:ss"));

    QList<QAudioDeviceInfo> audioInput = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QList<QAudioDeviceInfo> audioOutput = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
}

void MiscellaneousTimeCode::on_cbMidiDevicesIn_currentIndexChanged(int index)
{
    if (index == -1) return;

    UINT deviceID = ui->cbMidiDevicesIn->itemData(index).toUInt();
    this->MidiDevicesOpenIn(deviceID);
}

void MiscellaneousTimeCode::on_cbMidiDevicesOut_currentIndexChanged(int index)
{
    if (index == -1) return;

    UINT deviceID = ui->cbMidiDevicesOut->itemData(index).toUInt();
    this->MidiDevicesOpenOut(deviceID);
}

void MiscellaneousTimeCode::on_btnMtcStartStop_clicked()
{
    ui->pteMidiData->appendPlainText(QString("MTC %1 ......").arg(mbTimeCodeStarted ? "stop" : "start"));

    // MIDI 输入设备
    if (mHandleMidiIn == NULL)
    {
        UINT deviceID = ui->cbMidiDevicesIn->currentData().toUInt();
        this->MidiDevicesOpenIn(deviceID);
    }

    if (mHandleMidiIn != NULL && !mbTimeCodeInputOn)
    {
        MMRESULT res = midiInStart(mHandleMidiIn);
        if (res == MMSYSERR_NOERROR)
        {
            mbTimeCodeInputOn = true;
        }
        else
        {
            LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiInStart failed --> %s", this->MidiErrorCodeToString(res).c_str());
        }
    }

    // MIDI 输出设备
    if (mHandleMidiOut == NULL)
    {
        UINT deviceID = ui->cbMidiDevicesOut->currentData().toUInt();
        this->MidiDevicesOpenOut(deviceID);
    }

    if (mHandleMidiOut)
    {
        // 0x01 Stop
        // 0x02 Play
        // 0x03 Deferred Play
        // 0x04 Fast Forward
        // 0x05 Rewind
        // 0x06 Record Strobe(Punch In)
        // 0x07 Record Exit(Punch out)
        // 0x09 Pause

        HGLOBAL gMidiBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, 128);
        CHAR* pMidiBuffer = NULL;
        if (gMidiBuffer != NULL)
        {
            pMidiBuffer = (CHAR*)::GlobalLock(gMidiBuffer);
        }

        if (pMidiBuffer != NULL)
        {
            MIDIHDR midiHeader = { 0 };
            midiHeader.lpData = pMidiBuffer;
            midiHeader.dwBufferLength = 6;
            midiHeader.lpData[0] = 0xF0;
            midiHeader.lpData[1] = 0x7F;
            midiHeader.lpData[2] = 0x7F;
            midiHeader.lpData[3] = 0x06;
            midiHeader.lpData[4] = mbTimeCodeStarted ? 0x01 : 0x02; // 0x01:Stop  0x02:Play
            midiHeader.lpData[5] = 0xF7;

            midiOutPrepareHeader(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
            MMRESULT res = midiOutLongMsg(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
            if (res == MMSYSERR_NOERROR)
            {
                mbTimeCodeStarted = !mbTimeCodeStarted; // 启动 或 停止 成功，切换状态
                ui->btnMtcStartStop->setText(mbTimeCodeStarted ? "Stop" : "Start");
            }
            else
            {
                LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiOutLongMsg failed --> %s", this->MidiErrorCodeToString(res).c_str());
            }
            midiOutUnprepareHeader(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
        }

        if (gMidiBuffer != NULL)
        {
            ::GlobalUnlock(gMidiBuffer);
        }
        ::GlobalFree(gMidiBuffer);
    }
}

/*
 * mif4 不支持暂停操作
 */
void MiscellaneousTimeCode::on_btnMtcLocate_clicked()
{
    TimeCodeObj locateTimeCode = this->GetTimeCodeFromUI(ui->spbLocateHour, ui->spbLocateMinute, ui->spbLocateSecond, ui->spbLocateFrame);
    ui->lcdTimeCodeMTC->display(QString::fromStdString(std::to_string(locateTimeCode)));
    ui->pteMidiData->appendPlainText(QString("MTC locate at %1 ......").arg(QString::fromStdString(std::to_string(locateTimeCode))));

    if (mHandleMidiOut == NULL)
    {
        UINT deviceID = ui->cbMidiDevicesOut->currentData().toUInt();
        this->MidiDevicesOpenOut(deviceID);
    }

    if (mHandleMidiOut)
    {
        // 0x01 Stop
        // 0x02 Play
        // 0x03 Deferred Play
        // 0x04 Fast Forward
        // 0x05 Rewind
        // 0x06 Record Strobe(Punch In)
        // 0x07 Record Exit(Punch out)
        // 0x09 Pause

        HGLOBAL gMidiBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, 128);
        CHAR* pMidiBuffer = NULL;
        if (gMidiBuffer != NULL)
        {
            pMidiBuffer = (CHAR*)::GlobalLock(gMidiBuffer);
        }

        if (pMidiBuffer != NULL)
        {
            MIDIHDR midiHeader = { 0 };
            midiHeader.lpData = pMidiBuffer;
            midiHeader.dwBufferLength = 13;
            midiHeader.lpData[0]  = 0xF0;
            midiHeader.lpData[1]  = 0x7F;
            midiHeader.lpData[2]  = 0x7F;
            midiHeader.lpData[3]  = 0x06;
            midiHeader.lpData[4]  = 0x44;
            midiHeader.lpData[5]  = 0x06;
            midiHeader.lpData[6]  = 0x01;
            midiHeader.lpData[7]  = locateTimeCode.getHour()   & 0xFF;
            midiHeader.lpData[8]  = locateTimeCode.getMinute() & 0xFF;
            midiHeader.lpData[9]  = locateTimeCode.getSecond() & 0xFF;
            midiHeader.lpData[10] = locateTimeCode.getFrame()  & 0xFF;
            midiHeader.lpData[11] = 0x00;
            midiHeader.lpData[12] = 0xF7;
            midiOutPrepareHeader(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
            MMRESULT res = midiOutLongMsg(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
            if (res == MMSYSERR_NOERROR)
            {
                mbTimeCodeStarted = false; // 重定位后处于已停止状态，按钮设置为可启动状态
                ui->btnMtcStartStop->setText("Start");
            }
            else
            {
                LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiOutLongMsg failed --> %s", this->MidiErrorCodeToString(res).c_str());
            }
            midiOutUnprepareHeader(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
        }

        if (gMidiBuffer != NULL)
        {
            ::GlobalUnlock(gMidiBuffer);
        }
        ::GlobalFree(gMidiBuffer);
    }
}

//void MiscellaneousTimeCode::on_btnMtcStop_clicked()
//{
//    ui->pteMidiData->appendPlainText("MTC stop ......");
//
//    if (mHandleMidiOut == NULL)
//    {
//        UINT deviceID = ui->cbMidiDevicesOut->currentData().toUInt();
//        this->MidiDevicesOpenOut(deviceID);
//    }
//
//    if (mHandleMidiOut)
//    {
//        // 0x01 Stop
//        // 0x02 Play
//        // 0x03 Deferred Play
//        // 0x04 Fast Forward
//        // 0x05 Rewind
//        // 0x06 Record Strobe(Punch In)
//        // 0x07 Record Exit(Punch out)
//        // 0x09 Pause
//
//        HGLOBAL gMidiBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, 128);
//        CHAR* pMidiBuffer = NULL;
//        if (gMidiBuffer != NULL)
//        {
//            pMidiBuffer = (CHAR*)::GlobalLock(gMidiBuffer);
//        }
//
//        if (pMidiBuffer != NULL)
//        {
//            MIDIHDR midiHeader = { 0 };
//            midiHeader.lpData = pMidiBuffer;
//            midiHeader.dwBufferLength = 6;
//            midiHeader.lpData[0] = 0xF0;
//            midiHeader.lpData[1] = 0x7F;
//            midiHeader.lpData[2] = 0x7F;
//            midiHeader.lpData[3] = 0x06;
//            midiHeader.lpData[4] = 0x01;
//            midiHeader.lpData[5] = 0xF7;
//            midiOutPrepareHeader(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
//            MMRESULT res = midiOutLongMsg(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
//            if (res != MMSYSERR_NOERROR)
//            {
//                LogUtil::Debug(CODE_LOCATION, "MIDI OUT : midiOutLongMsg failed --> %s", this->MidiErrorCodeToString(res).c_str());
//            }
//            midiOutUnprepareHeader(mHandleMidiOut, &midiHeader, sizeof(MIDIHDR));
//        }
//
//        if (gMidiBuffer != NULL)
//        {
//            ::GlobalUnlock(gMidiBuffer);
//        }
//        ::GlobalFree(gMidiBuffer);
//    }
//}
