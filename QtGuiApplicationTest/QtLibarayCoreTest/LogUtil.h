#ifndef MGI_LOG_UTIL_H
#define MGI_LOG_UTIL_H

#include <QtCore/qglobal.h>
#if defined(QTLIBARAYCORETEST_LIB)
#define LOG_UTIL_EXPORT Q_DECL_EXPORT
#else
#define LOG_UTIL_EXPORT Q_DECL_IMPORT
#endif

#include <string>
#include <sstream>
#include <stdarg.h>
#include <map>

#define DEAFAULT_LOG_BUFFER_LENGTH 1024

#define CODE_LOCATION CodeLocation(__FILE__,__FUNCTION__,__LINE__)
#define CODE_LOCATION_STRING CodeLocation(__FILE__,__FUNCTION__,__LINE__).ToString()

#ifdef WIN32
#define DEAFAULT_LOG_FORMAT(buffer, fmt, length) \
va_list vaList; \
va_start(vaList, fmt); \
vsnprintf(buffer, DEAFAULT_LOG_BUFFER_LENGTH - 1, fmt, vaList); \
va_end(vaList);
#else
#define DEAFAULT_LOG_FORMAT(buffer, fmt, length) \
va_list vaList; \
va_start(vaList, fmt); \
snprintf(buffer, DEAFAULT_LOG_BUFFER_LENGTH - 1, fmt, vaList); \
va_end(vaList);
#endif

enum LogLevelType
{
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_ALARM,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_NONE
};

enum LogCollectionType
{
    LOG_COLLECTION_TYPE_0,
    LOG_COLLECTION_TYPE_1,
    LOG_COLLECTION_TYPE_2,
    LOG_COLLECTION_TYPE_MAX,
};

class LOG_UTIL_EXPORT CodeLocation
{
private:
    std::string m_strFileName;       // 源文件名(含绝对路径)
    std::string m_strShortFileName;  // 短文件名(无绝对路径)
    std::string m_strFuncName;       // 函数名（全名  namespace::class::MemberFunction）
    int m_nLineNumber;               // 行号

public:
    CodeLocation(const char* fileName, const char* funcName, int lineNumber);
    ~CodeLocation();

    int GetLineNumber();
    std::string GetFileName();
    std::string GetShortFileName();
    std::string GetShortClassName();
    std::string GetFunctionName();
    std::string GetFunctionNameFull();
    std::string ToString();
};

typedef void(*LogCallback)(LogLevelType level, CodeLocation *pLocation, std::string& msg);

/*!
    * \class LogUtil
    *
    * \brief  1. 记录日志（打印代码定位：函数、文件、行号）
    *         2. 收集采样数据
    * \author jiangchuanbiao
    * \date 六月 2018
    */
class LOG_UTIL_EXPORT LogUtil
{
private:
    LogUtil();
    static int              mLoggerID;
    static int              mLogCollections[LOG_COLLECTION_TYPE_MAX];
    static LogLevelType     mLevel;
    static LogCallback      mpLogCallback;

public:
    ~LogUtil();

    static void Init(LogLevelType level, bool bDisplayOnConsole, LogCallback zlogHandler = nullptr);

    static void Debug(const char *fmt, ...);
    static void Debug(CodeLocation location, const char *fmt, ...);

    static void Info(const char *fmt, ...);
    static void Info(CodeLocation location, const char *fmt, ...);

    static void Warn(const char *fmt, ...);
    static void Warn(CodeLocation location, const char *fmt, ...);

    static void Error(const char *fmt, ...);
    static void Error(CodeLocation location, const char *fmt, ...);

    static void Fatal(const char *fmt, ...);
    static void Fatal(CodeLocation location, const char *fmt, ...);

    static void CollectData(CodeLocation location, LogCollectionType collectType, const char* format, ...);

    static void DefaultLogHandler(LogLevelType level, CodeLocation *pLocation, std::string& msg);
    static std::string ConvertToString(std::wstring wstring);
private:
    static std::string GetSystemTimeString();
    static std::string GetLevelString(LogLevelType logLevel);
    static void Log(LogLevelType level, std::string timestamp, std::string message);
    static void Log(LogLevelType level, std::string timestamp, CodeLocation location, std::string message);
};

#endif //MGI_LOG_UTIL_H
