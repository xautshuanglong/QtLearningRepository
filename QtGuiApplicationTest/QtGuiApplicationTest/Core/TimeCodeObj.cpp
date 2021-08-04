#include "TimecodeObj.h"

#include <iomanip>
#include <QMetaType>

#define HOUR_PER_DATY     24    // 1 * 24
#define MINUTE_PER_DAY    1440  // 1 * 24 * 60
#define MINUTE_PER_HOUR   60    // 1 * 60
#define SECOND_PER_DAY    86400 // 1 * 24 * 60 * 60
#define SECOND_PER_HOUR   3600  // 1 * 60 * 60
#define SECOND_PER_MINUTE 60    // 1 * 60

namespace std
{
    string to_string(const TimeCodeObj& obj)
    {
        std::string retValue = "";

        int hour = obj.getHour();
        int minute = obj.getMinute();
        int second = obj.getSecond();
        int frame = obj.getFrame();

        retValue += std::string(hour <= 9 ? "0" : "") + to_string(hour) + ":";
        retValue += std::string(minute <= 9 ? "0" : "") + to_string(minute) + ":";
        retValue += std::string(second <= 9 ? "0" : "") + to_string(second) + ":";
        retValue += std::string(frame <= 9 ? "0" : "") + to_string(frame);

        return retValue;
    }
}

TimeCodeObj::TimeCodeObj() noexcept
    : m_iHour(0)
    , m_iMinute(0)
    , m_iSecond(0)
    , m_iFrame(0)
    , m_iFrameRate(30)
{
    qRegisterMetaType<TimeCodeObj>("TimeCodeObj");
}

TimeCodeObj::TimeCodeObj(int hour, int minute, int second, int frame, int frameRate) noexcept
    : m_iHour(hour)
    , m_iMinute(minute)
    , m_iSecond(second)
    , m_iFrame(frame)
    , m_iFrameRate(frameRate)
{
    qRegisterMetaType<TimeCodeObj>("TimeCodeObj");
}

TimeCodeObj::TimeCodeObj(const TimeCodeObj& other) noexcept
{
    m_iHour = other.m_iHour;
    m_iMinute = other.m_iMinute;
    m_iSecond = other.m_iSecond;
    m_iFrame = other.m_iFrame;
    m_iFrameRate = other.m_iFrameRate;
}

TimeCodeObj::TimeCodeObj(TimeCodeObj&& other) noexcept
{
    m_iHour = other.m_iHour;
    m_iMinute = other.m_iMinute;
    m_iSecond = other.m_iSecond;
    m_iFrame = other.m_iFrame;
    m_iFrameRate = other.m_iFrameRate;
}

TimeCodeObj::~TimeCodeObj()
{}

TimeCodeObj& TimeCodeObj::operator=(const TimeCodeObj& other)
{
    m_iHour = other.m_iHour;
    m_iMinute = other.m_iMinute;
    m_iSecond = other.m_iSecond;
    m_iFrame = other.m_iFrame;
    m_iFrameRate = other.m_iFrameRate;
    return *this;
}

bool TimeCodeObj::operator <(const TimeCodeObj& other) const
{
    if (m_iHour != other.m_iHour)
    {
        return m_iHour < other.m_iHour;
    }
    if (m_iMinute != other.m_iMinute)
    {
        return m_iMinute < other.m_iMinute;
    }
    if (m_iSecond != other.m_iSecond)
    {
        return m_iSecond < other.m_iSecond;
    }
    return m_iFrame < other.m_iFrame;
}

bool TimeCodeObj::operator >(const TimeCodeObj& other) const
{
    if (m_iHour != other.m_iHour)
    {
        return m_iHour > other.m_iHour;
    }
    if (m_iMinute != other.m_iMinute)
    {
        return m_iMinute > other.m_iMinute;
    }
    if (m_iSecond != other.m_iSecond)
    {
        return m_iSecond > other.m_iSecond;
    }
    return m_iFrame > other.m_iFrame;
}

bool TimeCodeObj::operator ==(const TimeCodeObj& other) const
{
    if (m_iHour == other.m_iHour &&
        m_iMinute == other.m_iMinute &&
        m_iSecond == other.m_iSecond &&
        m_iFrame == other.m_iFrame)
    {
        return true;
    }
    return false;
}

bool TimeCodeObj::operator <=(const TimeCodeObj& other) const
{
    if (*this > other)
    {
        return false;
    }
    return true;
}

bool TimeCodeObj::operator >=(const TimeCodeObj& other) const
{
    if (*this < other)
    {
        return false;
    }
    return true;
}

void TimeCodeObj::addSecond(int second)
{
    int secondThis = this->toSecond();
    secondThis += second;
    this->fromSecond(secondThis);
}

void TimeCodeObj::subSecond(int second)
{
    int secondThis = this->toSecond();
    secondThis -= second;
    this->fromSecond(secondThis);
}

void TimeCodeObj::fromSecond(int second)
{
    int tempSecond = second;
    m_iFrame = 0;
    m_iFrameRate = 0;

    m_iHour = tempSecond / SECOND_PER_HOUR;
    tempSecond -= m_iHour * SECOND_PER_HOUR;
    m_iHour = m_iHour % 24;
    m_iMinute = tempSecond / SECOND_PER_MINUTE;
    m_iSecond = tempSecond - m_iMinute * SECOND_PER_MINUTE;
}

int TimeCodeObj::toSecond() const
{
    int retValue = m_iSecond;
    retValue += m_iMinute * SECOND_PER_MINUTE;
    retValue += m_iHour * SECOND_PER_HOUR;
    return retValue;
}

int TimeCodeObj::secondsTo(const TimeCodeObj& other) const
{
    int secondThis = this->toSecond();
    int secondOther = other.toSecond();
    return secondThis - secondOther;
}

std::ostream& operator<<(std::ostream& output, const TimeCodeObj& obj)
{
    output << std::setw(2) << std::setfill('0') << obj.m_iHour << ":"
        << std::setw(2) << std::setfill('0') << obj.m_iMinute << ":"
        << std::setw(2) << std::setfill('0') << obj.m_iSecond << ":"
        << std::setw(2) << std::setfill('0') << obj.m_iFrame;
    return output;
}
