#ifndef TIMECODE_OBJ_H
#define TIMECODE_OBJ_H

#include <ostream>

class TimeCodeObj
{
public:
    TimeCodeObj() noexcept;
    TimeCodeObj(int hour, int minute, int second, int frame, int frameRate) noexcept;
    TimeCodeObj(const TimeCodeObj& other) noexcept;
    TimeCodeObj(TimeCodeObj&& other) noexcept;
    ~TimeCodeObj();

    TimeCodeObj& operator=(const TimeCodeObj& other);

    bool operator <(const TimeCodeObj& other) const;
    bool operator >(const TimeCodeObj& other) const;
    bool operator ==(const TimeCodeObj& other) const;
    bool operator <=(const TimeCodeObj& other) const;
    bool operator >=(const TimeCodeObj& other) const;

    inline void setHour(int hour) { m_iHour = hour; }
    inline void setMinute(int minute) { m_iMinute = minute; }
    inline void setSecond(int second) { m_iSecond = second; }
    inline void setFrame(int frame) { m_iFrame = frame; }
    inline void setFrameRate(int frameRate) { m_iFrameRate = frameRate; }

    inline int getHour() const { return m_iHour; }
    inline int getMinute() const { return m_iMinute; }
    inline int getSecond() const { return m_iSecond; }
    inline int getFrame() const { return m_iFrame; }
    inline int getFrameRate() const { return m_iFrameRate; }

    void addSecond(int second);
    void subSecond(int second);
    void fromSecond(int second);
    int  toSecond() const;
    int  secondsTo(const TimeCodeObj& other) const;

    friend std::ostream& operator<<(std::ostream& output, const TimeCodeObj& obj);

private:
    int m_iHour;
    int m_iMinute;
    int m_iSecond;
    int m_iFrame;
    int m_iFrameRate; // frame per second
};

namespace std
{
    string to_string(const TimeCodeObj& obj);

    template<>
    class hash<TimeCodeObj>
    {
    public:
        size_t operator()(const TimeCodeObj& obj) const
        {
            return
                hash<int>()(obj.getHour()) ^
                hash<int>()(obj.getMinute()) ^
                hash<int>()(obj.getSecond()) ^
                hash<int>()(obj.getFrame());
        }
    };
}

#endif // TIMECODE_OBJ_H
