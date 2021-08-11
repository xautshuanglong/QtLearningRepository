#include "MiscellaneousTimeCode.h"
#include "ui_MiscellaneousTimeCode.h"

#include <QDir>
#include <QMetaEnum>
#include <QTimer>
#include <QDateTime>
#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QAudioInput>
#include <QAudioOutput>

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
    , mbAudioInputStarted(false)
    , mbAudioInputChanged(false)
    , mbAudioOutputChanged(false)
    , mHandleMidiIn(NULL)
    , mHandleMidiOut(NULL)
    , mpAudioInput(Q_NULLPTR)
    , mpAudioOutput(Q_NULLPTR)
    , mpAudioDeviceIO(Q_NULLPTR)
{
    ui->setupUi(this);

    this->AudioEnumerateDevices();
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
    // 关闭音频输入设备读取操作
    if (mpAudioInput && mpAudioInput->state() != QAudio::StoppedState)
    {
        mpAudioInput->stop();
    }
    this->CloseFile(mpFilePcmAudioIn);

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
    // 时间码显示 LCD
    ui->lcdTimeCodeMTC->display("00:00:00:00");
    ui->lcdTimeCodeLTC->display("00:00:00:00");
    // 音频输入输出设备
    this->UpdateComboxAudio();
    // MIDI 设备列表选项
    this->UpdateComboxMidi();

    // 时间码触发器
    mpTimeCodeEmiter = new QTimer(this);
    mpTimeCodeEmiter->setInterval(std::chrono::milliseconds(1));
    mpTimeCodeEmiter->setTimerType(Qt::PreciseTimer);
    Qt::TimerType timerType = mpTimeCodeEmiter->timerType();
    connect(mpTimeCodeEmiter, SIGNAL(timeout()), this, SLOT(TimeCodeEmiter_TimeOut()));
}

void MiscellaneousTimeCode::UpdateComboxAudio()
{

    QHash<QString, QAudioDeviceInfo>::const_iterator audioIt = mHashAudioInput.begin();
    QHash<QString, QAudioDeviceInfo>::const_iterator audioEnd = mHashAudioInput.end();
    ui->cbAudioDevicesIn->clear();
    ui->cbAudioDevicesIn->addItem("None", QVariant::fromValue(QAudioDeviceInfo()));
    while (audioIt != audioEnd)
    {
        ui->cbAudioDevicesIn->addItem(audioIt.key(), QVariant::fromValue(audioIt.value()));
        ++audioIt;
    }

    QHashIterator<QString, QAudioDeviceInfo> audioOutput(mHashAudioOutput);
    ui->cbAudioDevicesOut->clear();
    ui->cbAudioDevicesOut->addItem("None", QVariant::fromValue(QAudioDeviceInfo()));
    while (audioOutput.hasNext())
    {
        audioOutput.next();
        ui->cbAudioDevicesOut->addItem(audioOutput.key(), QVariant::fromValue(audioOutput.value()));
    }
}

void MiscellaneousTimeCode::UpdateComboxMidi()
{
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

void MiscellaneousTimeCode::CloseFile(QFile& file)
{
    if (file.isOpen())
    {
        file.flush();
        file.close();
    }
}

void MiscellaneousTimeCode::OpenFile(QFile& file)
{
    QString dirPath = qApp->applicationDirPath();
    dirPath += "/audio_pcm";
    QDir dir(dirPath);
    if (!dir.exists())
    {
        dir.mkpath(dirPath);
    }

    QString filename = dirPath;
    filename += "/LTC_PCM_";
    filename += QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    filename += ".raw";

    file.setFileName(filename);
    file.open(QIODevice::WriteOnly);
}

void MiscellaneousTimeCode::AudioEnumerateDevices()
{
    mHashAudioInput.clear();
    QList<QAudioDeviceInfo> audioInput = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for (QAudioDeviceInfo audioInfo : audioInput)
    {
        mHashAudioInput.insert(audioInfo.deviceName(), audioInfo);
    }

    mHashAudioOutput.clear();
    QList<QAudioDeviceInfo> audioOutput = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for (QAudioDeviceInfo audioInfo : audioOutput)
    {
        mHashAudioOutput.insert(audioInfo.deviceName(), audioInfo);
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

std::string MiscellaneousTimeCode::AudioErrorToString(QAudio::Error error)
{
    std::string strAudioError;
    switch (error)
    {
    case QAudio::NoError:
        strAudioError = "QAudio::NoError";
        break;
    case QAudio::OpenError:
        strAudioError = "QAudio::OpenError";
        break;
    case QAudio::IOError:
        strAudioError = "QAudio::IOError";
        break;
    case QAudio::UnderrunError:
        strAudioError = "QAudio::UnderrunError";
        break;
    case QAudio::FatalError:
        strAudioError = "QAudio::FatalError";
        break;
    default:
        strAudioError = "UNKONWN";
        break;
    }
    return strAudioError;
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

void MiscellaneousTimeCode::SlotAudioInputDestroyed(QObject* pObj)
{
    LogUtil::Debug(CODE_LOCATION, "Audio device input destroyed 0x%08X", pObj);
}

void MiscellaneousTimeCode::SlotAudioInputStateChanged(QAudio::State state)
{
    QString strAudioState;
    switch (state)
    {
    case QAudio::ActiveState:
        strAudioState = "QAudio::ActiveState";
        break;
    case QAudio::SuspendedState:
        strAudioState = "QAudio::SuspendedState";
        break;
    case QAudio::StoppedState:
        strAudioState = "QAudio::StoppedState";
        break;
    case QAudio::IdleState:
        strAudioState = "QAudio::IdleState";
        break;
    case QAudio::InterruptedState:
        strAudioState = "QAudio::InterruptedState";
        break;
    default:
        strAudioState = "UNKNOWN";
        break;
    }
    LogUtil::Info(CODE_LOCATION, "Audio input state changed.  state=%s", strAudioState.toUtf8().data());
}

void MiscellaneousTimeCode::SlotAudioIoDestroyed(QObject *pObj /* = Q_NULLPTR */)
{
    LogUtil::Debug(CODE_LOCATION, "Audio device io destroyed 0x%08X", pObj);
}

void MiscellaneousTimeCode::SlotAudioIoAboutToClose()
{
    LogUtil::Debug(CODE_LOCATION, "Audio device io about to close ...");
}

void MiscellaneousTimeCode::SlotAudioIoBytesWritten(qint64 bytes)
{
    LogUtil::Debug(CODE_LOCATION, "Audio device io bytes written %lld ...", bytes);
}

void MiscellaneousTimeCode::SlotAudioIoReadyRead()
{
    if (mpAudioDeviceIO == Q_NULLPTR) return;

    QByteArray audioBytes = mpAudioDeviceIO->readAll();
    if (mpFilePcmAudioIn.isOpen())
    {
        mpFilePcmAudioIn.write(audioBytes);
    }
}

void MiscellaneousTimeCode::SlotAudioIoChannelBytesWritten(int channel, qint64 bytes)
{
    LogUtil::Debug(CODE_LOCATION, "Audio device io channel bytes written channel:%d bytes:%lld ...", channel, bytes);
}

void MiscellaneousTimeCode::SlotAudioIoChannelReadyRead(int channel)
{
    LogUtil::Debug(CODE_LOCATION, "Audio device io channel ready read channel:%d ...", channel);
}

void MiscellaneousTimeCode::SlotAudioIoReadChannelFinished()
{
    LogUtil::Debug(CODE_LOCATION, "Audio device io channel read finished ...");
}

void MiscellaneousTimeCode::on_btnTransferTest_clicked()
{
    QDateTime testTime(QDate(2021, 8, 4), QTime(15, 30, 20, 0));
    QString timeString_1 = testTime.toString("yyyy-MM-dd hh:mm:ss");

    QDateTime testTimeBig(QDate(2021, 8, 4), QTime(16, 32, 25, 0));
    int secondDiff = testTimeBig.secsTo(testTime);

    testTime.addSecs(50);
    QString timeString_2 = testTime.toString("yyyy-MM-dd hh:mm:ss");

    TimeCodeObj test1(1, 2, 3, 0, 0);
    test1.addSecond(3667);
    std::string testStr1 = std::to_string(test1);

    TimeCodeObj test2(1, 2, 3, 0, 0);
    test2.subSecond(53);
    std::string testStr2 = std::to_string(test2);

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

void MiscellaneousTimeCode::on_btnEnumerateDevices_clicked()
{
    // 重新枚举音频设备
    this->AudioEnumerateDevices();
    this->UpdateComboxAudio();
    // 重新枚举 MIDI 设备
    this->MidiEnumerateDevices();
    this->UpdateComboxMidi();
}

void MiscellaneousTimeCode::on_cbAudioDevicesIn_currentIndexChanged(int index)
{
    if (index == -1) return;

    QVariant userData = ui->cbAudioDevicesIn->currentData();
    if (!userData.isNull() && userData.isValid())
    {
        mAudioDeviceInfoIn = userData.value<QAudioDeviceInfo>();
        mbAudioInputChanged = true;
    }
}

void MiscellaneousTimeCode::on_cbAudioDevicesOut_currentIndexChanged(int index)
{
    if (index == -1) return;

    QVariant userData = ui->cbAudioDevicesOut->currentData();
    if (!userData.isNull() && userData.isValid())
    {
        mAudioDeviceInfoOut = userData.value<QAudioDeviceInfo>();
        mbAudioOutputChanged = true;
    }
}

void MiscellaneousTimeCode::on_btnLtcStartStop_clicked()
{
    if (mAudioDeviceInfoIn.isNull())
    {
        LogUtil::Warn(CODE_LOCATION, "Audio device info is NULL");
        return;
    }

    // 音频格式  LTC  双声道  48KHZ  有符号8bit （QAudio 实测检验）
    QAudioFormat testFormat;
    testFormat.setSampleRate(44100);
    testFormat.setByteOrder(QAudioFormat::LittleEndian);
    testFormat.setSampleType(QAudioFormat::SignedInt);
    testFormat.setChannelCount(1);
    testFormat.setSampleSize(8);
    testFormat.setCodec("audio/pcm"); // Linear PCM

    QList<int> sampleRates = mAudioDeviceInfoIn.supportedSampleRates();
    QList<QAudioFormat::Endian> endians = mAudioDeviceInfoIn.supportedByteOrders();
    QList<QAudioFormat::SampleType> sampleTypes = mAudioDeviceInfoIn.supportedSampleTypes();
    QList<int> channelCounts = mAudioDeviceInfoIn.supportedChannelCounts();
    QList<int> sizes = mAudioDeviceInfoIn.supportedSampleSizes();
    QStringList codecs = mAudioDeviceInfoIn.supportedCodecs();

    if (!mAudioDeviceInfoIn.isFormatSupported(testFormat))
    {
        LogUtil::Warn(CODE_LOCATION, "Audio format is not supported. Will reset as nearest format ...");
        // Old Format
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat Old   SampleRate: %d", testFormat.sampleRate());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat Old    ByteOrder: %d", testFormat.byteOrder());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat Old   SampleType: %d", testFormat.sampleType());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat Old ChannelCount: %d", testFormat.channelCount());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat Old   SampleSize: %d", testFormat.sampleSize());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat Old        Codec: %s", testFormat.codec().toUtf8().data());

        mAudioFormatIn = mAudioDeviceInfoIn.nearestFormat(testFormat);

        // New Format
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat New   SampleRate: %d", mAudioFormatIn.sampleRate());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat New    ByteOrder: %d", mAudioFormatIn.byteOrder());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat New   SampleType: %d", mAudioFormatIn.sampleType());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat New ChannelCount: %d", mAudioFormatIn.channelCount());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat New   SampleSize: %d", mAudioFormatIn.sampleSize());
        LogUtil::Debug(CODE_LOCATION, "QAudioFormat New        Codec: %s", mAudioFormatIn.codec().toUtf8().data());
    }

    if (mbAudioInputChanged)
    {
        mbAudioInputChanged = false;

        if (mpAudioInput != Q_NULLPTR)
        {
            mpAudioInput->deleteLater();
            mpAudioInput = Q_NULLPTR;
        }
    }
    if (mpAudioInput == Q_NULLPTR)
    {
        mpAudioInput = new QAudioInput(mAudioDeviceInfoIn, mAudioFormatIn, this);
        QString devName = mAudioDeviceInfoIn.deviceName();
        LogUtil::Debug(CODE_LOCATION, "Audio device input create 0x%08X %s", mpAudioInput, devName.toUtf8().data());
        this->connect(mpAudioInput, SIGNAL(destroyed(QObject*)), this, SLOT(SlotAudioInputDestroyed(QObject*)));
        this->connect(mpAudioInput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(SlotAudioInputStateChanged(QAudio::State)));
        //this->connect(mpAudioInput, SIGNAL(notify()), this, SLOT(SlotAudioInputNotify())); // This signal is emitted when x ms of audio data has been processed the interval set by setNotifyInterval(x).

        this->CloseFile(mpFilePcmAudioIn);
        this->OpenFile(mpFilePcmAudioIn);
    }

    if (mbAudioInputStarted)
    {
        mpAudioInput->stop();
        LogUtil::Debug(CODE_LOCATION, "Audio input after stop() error=%s", this->AudioErrorToString(mpAudioInput->error()).c_str());
    }
    else
    {
        mpAudioDeviceIO = mpAudioInput->start();
        LogUtil::Debug(CODE_LOCATION, "Audio input after start() error=%s", this->AudioErrorToString(mpAudioInput->error()).c_str());
        LogUtil::Debug(CODE_LOCATION, "Audio device io created 0x%08X", mpAudioDeviceIO);
        this->connect(mpAudioDeviceIO, SIGNAL(destroyed(QObject*)), this, SLOT(SlotAudioIoDestroyed(QObject*)));
        this->connect(mpAudioDeviceIO, SIGNAL(aboutToClose()), this, SLOT(SlotAudioIoAboutToClose()));
        this->connect(mpAudioDeviceIO, SIGNAL(bytesWritten(qint64)), this, SLOT(SlotAudioIoBytesWritten(qint64)));
        this->connect(mpAudioDeviceIO, SIGNAL(readyRead()), this, SLOT(SlotAudioIoReadyRead()));
        this->connect(mpAudioDeviceIO, SIGNAL(channelReadyRead(int )), this, SLOT(SlotAudioIoChannelReadyRead(int)));
        this->connect(mpAudioDeviceIO, SIGNAL(readChannelFinished()), this, SLOT(SlotAudioIoReadChannelFinished()));
    }
    mbAudioInputStarted = !mbAudioInputStarted;
    ui->btnLtcStartStop->setText(mbAudioInputStarted ? "Stop" : "Start");
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
