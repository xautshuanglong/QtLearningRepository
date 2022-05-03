#include "MiscellaneousWinSystemInfo.h"

#include "JCB_Logger/LogUtil.h"

MiscellaneousWinSystemInfo::MiscellaneousWinSystemInfo(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousWinSystemInfo::~MiscellaneousWinSystemInfo()
{
}

QString MiscellaneousWinSystemInfo::GetTitle()
{
    return QObject::tr("System Information");
}

QString MiscellaneousWinSystemInfo::GetTitleTooltip()
{
    return QObject::tr("How to use windows API get system information.");
}

MiscellaneousTestGroup MiscellaneousWinSystemInfo::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinSystemInfo::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_SystemInfo;
}

void MiscellaneousWinSystemInfo::on_btnSystemInfo_clicked()
{
    //DWORD tickCount = GetTickCount();
    ULONGLONG tickCount64 = GetTickCount64();
    std::string tickCountStr = this->TimeSpanFromMillisecond(tickCount64);

    ULONGLONG interruptTime = 0;
    QueryInterruptTime(&interruptTime);
    ULONGLONG interruptTimeMS = interruptTime / 10000;
    std::string interruptTimeMSStr = this->TimeSpanFromMillisecond(interruptTimeMS);

    ULONGLONG unbiasedInterruptTime = 0;
    if (QueryUnbiasedInterruptTime(&unbiasedInterruptTime))
    {
        ULONGLONG tickCount64MS = unbiasedInterruptTime / 10000;
        std::string tickCount64MSStr = this->TimeSpanFromMillisecond(tickCount64MS);
        int i = 0;
    }
    else
    {
        DWORD errorCode = GetLastError();
        int i = 0;
    }
    int i = 0;
}

void MiscellaneousWinSystemInfo::on_btnTotalMemory_clicked()
{
    int i = 0;
}

void MiscellaneousWinSystemInfo::on_btnEmptyTest1_clicked()
{
    int i = 0;
}

void MiscellaneousWinSystemInfo::on_btnEmptyTest2_clicked()
{
    int i = 0;
}

std::string MiscellaneousWinSystemInfo::TimeSpanFromMillisecond(ULONGLONG milliSeconds)
{
    std::string retValue;

    ULONGLONG tempSeconds = milliSeconds / 1000;
    ULONGLONG tempMinutes = tempSeconds / 60;
    ULONGLONG tempHours = tempMinutes / 60;
    ULONGLONG tempDays = tempHours / 24;

    ULONGLONG milliSecond = milliSeconds % 1000;
    ULONGLONG second = tempSeconds % 60;
    ULONGLONG minute = tempMinutes % 60;
    ULONGLONG hour = tempHours % 24;
    ULONGLONG day = tempDays;

    std::stringstream timeStr;
    timeStr << std::setfill('0') << std::setw(3) << day << ":";
    timeStr << std::setfill('0') << std::setw(2) << hour << ":";
    timeStr << std::setfill('0') << std::setw(2) << minute << ":";
    timeStr << std::setfill('0') << std::setw(3) << second << ".";
    timeStr << std::setfill('0') << std::setw(3) << milliSecond;
    retValue = timeStr.str();

    return retValue;
}
