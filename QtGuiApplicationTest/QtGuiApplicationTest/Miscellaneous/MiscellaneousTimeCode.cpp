#include "MiscellaneousTimeCode.h"
#include "ui_MiscellaneousTimeCode.h"

#include <QTimer>

#include "Utils/TimeUtil.h"
#include "LogUtil.h"

using namespace Shuanglong::Utils;

MiscellaneousTimeCode::MiscellaneousTimeCode(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousTimeCode())
    , mbTimeCodeEnable(false)
{
    ui->setupUi(this);

    // UI 控件初始化
    mBtnTimeEmiterText = ui->btnTimeEmiterTest->text();
    ui->btnTimeEmiterTest->setText(QString("%1 (disable)").arg(mBtnTimeEmiterText));

    // 时间码触发器
    mpTimeCodeEmiter = new QTimer(this);
    mpTimeCodeEmiter->setInterval(std::chrono::milliseconds(1));
    connect(mpTimeCodeEmiter, SIGNAL(timeout()), this, SLOT(TimeCodeEmiter_TimeOut()));

    // 高性能计数器
    mCurFrequency = TimeUtil::QueryPerformanceFrequency();
}

MiscellaneousTimeCode::~MiscellaneousTimeCode()
{
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
    UINT midiInNum = midiInGetNumDevs();
    for (UINT i = 0; i < midiInNum; ++i)
    {
        MIDIINCAPS midiCaps;
        midiInGetDevCaps(i, &midiCaps, sizeof(MIDIINCAPS));
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
}
