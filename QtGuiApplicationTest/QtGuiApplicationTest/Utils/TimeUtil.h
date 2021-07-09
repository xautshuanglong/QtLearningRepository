#pragma once

/**************************************************************************\
 *  FileName: TimeUtil.h
 *
 *  Author  : JiangShuanglong
 *  Date    : 2017-01-03 22:31:56
 *
 *  Description: some operation about time include get current timestamp
 *  and format time string.
 *
 *   %a simplified weekday
 *   %A full name of weekday
 *   %b simplified month
 *   %B full name of month
 *   %c standard date time string
 *   %C year after two
 *   %d decimal representation which day of a month
 *   %D 月/天/年
 *   %e 在两字符域中，十进制表示的每月的第几天
 *   %F 年-月-日
 *   %g 年份的后两位数字，使用基于周的年
 *   %G 年分，使用基于周的年
 *   %h 简写的月份名
 *   %H 24小时制的小时
 *   %I 12小时制的小时
 *   %j 十进制表示的每年的第几天
 *   %m 十进制表示的月份
 *   %M 十时制表示的分钟数
 *   %n 新行符
 *   %p 本地的AM或PM的等价显示
 *   %r 12小时的时间
 *   %R 显示小时和分钟：hh:mm
 *   %S 十进制的秒数
 *   %t 水平制表符
 *   %T 显示时分秒：hh:mm:ss
 *   %u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
 *   %U 第年的第几周，把星期日做为第一天（值从0到53）
 *   %V 每年的第几周，使用基于周的年
 *   %w 十进制表示的星期几（值从0到6，星期天为0）
 *   %W 每年的第几周，把星期一做为第一天（值从0到53）
 *   %x 标准的日期串
 *   %X 标准的时间串
 *   %y 不带世纪的十进制年份（值从0到99）
 *   %Y 带世纪部分的十制年份
 *   %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
 *   %% 百分号
 * =============================== MSDN ===================================
 *	%a : Abbreviated weekday name
 *	%A : Full weekday name
 *	%b : Abbreviated month name
 *	%B : Full month name
 *	%c : Date and time representation appropriate for locale
 *	%d : Day of month as decimal number (01 – 31)
 *	%H : Hour in 24-hour format (00 – 23)
 *	%I : Hour in 12-hour format (01 – 12)
 *	%j : Day of year as decimal number (001 – 366)
 *	%m : Month as decimal number (01 – 12)
 *	%M : Minute as decimal number (00 – 59)
 *	%p : Current locale's A.M./P.M. indicator for 12-hour clock
 *	%S : Second as decimal number (00 – 59)
 *	%U : Week of year as decimal number, with Sunday as first day of week (00 – 53)
 *	%w : Weekday as decimal number (0 – 6; Sunday is 0)
 *	%W : Week of year as decimal number, with Monday as first day of week (00 – 53)
 *	%x : Date representation for current locale
 *	%X : Time representation for current locale
 *	%y : Year without century, as decimal number (00 – 99)
 *	%Y : Year with century, as decimal number
 *	%z, %Z : Either the time-zone name or time zone abbreviation, depending on registry settings; no characters if time zone is unknown
 *	%% : Percent sign
\**************************************************************************/

#include <windows.h>
#include <ctime>
#include <iostream>

namespace Shuanglong::Utils
{
    enum FileTimeType
    {
        FILETIME_TYPE_CREATION,
        FILETIME_TYPE_LASTACCESS,
        FILETIME_TYPE_LASTWRITE
    };

    //************************************************************************
    //    Author     : JiangChuanbiao
    //    Date       : 2017-1-4
    //    Description: get timestamp and format date time string.
    //************************************************************************
    class TimeUtil sealed
    {
    private:
        static tm m_tmCurTime;
        static time_t m_tCurTime;
        static SYSTEMTIME m_systemTime;

    public:
        TimeUtil() = delete;
        TimeUtil(const TimeUtil& originalObj) = delete;
        TimeUtil(TimeUtil&& originalObj) = delete;
        ~TimeUtil();

        static std::string GetDateString();
        static std::string GetTimeString();
        static std::string GetDateTimeString();
        static std::string GetFormatTimeString(const char* timeFormat);

        static std::string GetFullTimestampString();
        static std::string GetSimpleTimestampString();
        static LONGLONG GetCurrentTimestamp();

        static LONGLONG QueryPerformanceCounter();
        static LONGLONG QueryPerformanceFrequency();
        static void IncreaseTimerPrecision();

        // 关于文件时间
        static LONGLONG GetFileTimeByType(std::string fullFilename, FileTimeType timeType);

    private:
        static void GetMyCurrentTime();
    };
}

