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
 *   %D ��/��/��
 *   %e �����ַ����У�ʮ���Ʊ�ʾ��ÿ�µĵڼ���
 *   %F ��-��-��
 *   %g ��ݵĺ���λ���֣�ʹ�û����ܵ���
 *   %G ��֣�ʹ�û����ܵ���
 *   %h ��д���·���
 *   %H 24Сʱ�Ƶ�Сʱ
 *   %I 12Сʱ�Ƶ�Сʱ
 *   %j ʮ���Ʊ�ʾ��ÿ��ĵڼ���
 *   %m ʮ���Ʊ�ʾ���·�
 *   %M ʮʱ�Ʊ�ʾ�ķ�����
 *   %n ���з�
 *   %p ���ص�AM��PM�ĵȼ���ʾ
 *   %r 12Сʱ��ʱ��
 *   %R ��ʾСʱ�ͷ��ӣ�hh:mm
 *   %S ʮ���Ƶ�����
 *   %t ˮƽ�Ʊ��
 *   %T ��ʾʱ���룺hh:mm:ss
 *   %u ÿ�ܵĵڼ��죬����һΪ��һ�� ��ֵ��0��6������һΪ0��
 *   %U ����ĵڼ��ܣ�����������Ϊ��һ�죨ֵ��0��53��
 *   %V ÿ��ĵڼ��ܣ�ʹ�û����ܵ���
 *   %w ʮ���Ʊ�ʾ�����ڼ���ֵ��0��6��������Ϊ0��
 *   %W ÿ��ĵڼ��ܣ�������һ��Ϊ��һ�죨ֵ��0��53��
 *   %x ��׼�����ڴ�
 *   %X ��׼��ʱ�䴮
 *   %y �������͵�ʮ������ݣ�ֵ��0��99��
 *   %Y �����Ͳ��ֵ�ʮ�����
 *   %z��%Z ʱ�����ƣ�������ܵõ�ʱ�������򷵻ؿ��ַ���
 *   %% �ٷֺ�
 * =============================== MSDN ===================================
 *	%a : Abbreviated weekday name
 *	%A : Full weekday name
 *	%b : Abbreviated month name
 *	%B : Full month name
 *	%c : Date and time representation appropriate for locale
 *	%d : Day of month as decimal number (01 �C 31)
 *	%H : Hour in 24-hour format (00 �C 23)
 *	%I : Hour in 12-hour format (01 �C 12)
 *	%j : Day of year as decimal number (001 �C 366)
 *	%m : Month as decimal number (01 �C 12)
 *	%M : Minute as decimal number (00 �C 59)
 *	%p : Current locale's A.M./P.M. indicator for 12-hour clock
 *	%S : Second as decimal number (00 �C 59)
 *	%U : Week of year as decimal number, with Sunday as first day of week (00 �C 53)
 *	%w : Weekday as decimal number (0 �C 6; Sunday is 0)
 *	%W : Week of year as decimal number, with Monday as first day of week (00 �C 53)
 *	%x : Date representation for current locale
 *	%X : Time representation for current locale
 *	%y : Year without century, as decimal number (00 �C 99)
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

        // �����ļ�ʱ��
        static LONGLONG GetFileTimeByType(std::string fullFilename, FileTimeType timeType);

    private:
        static void GetMyCurrentTime();
    };
}

