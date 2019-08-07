#include "LogUtil.h"

#include <sstream>
#include <iostream>
#include <codecvt>
#include <ctime>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "zlog.h"

int                LogUtil::mLoggerID = 0;
int                LogUtil::mLogCollections[LOG_COLLECTION_TYPE_MAX] = { 0 };
LogCallback       LogUtil::mpLogCallback = nullptr;
LogLevelType       LogUtil::mLevel = LOG_LEVEL_NONE;

LogUtil::LogUtil()
{
    ;
}

LogUtil::~LogUtil()
{
}

void LogUtil::Init(LogLevelType level, LogCallback zlogHandler/* =/* =nullptr */)
{
    mLevel = level;
    if (zlogHandler != nullptr)
    {
        mpLogCallback = zlogHandler;
        return;
    }

    mpLogCallback = DefaultLogHandler;

    // zlog 初始化设置
    if (mLoggerID == 0)
    {
        mLoggerID = zlog::IZLogManager::getRef().createLogger("LogUtil");
    }
    zlog::IZLogManager::getRef().setLogFilePath(mLoggerID, "./logs/");
    zlog::IZLogManager::getRef().setFilterLogLevel(mLoggerID, level);
    zlog::IZLogManager::getRef().setLogFileSizeLimit(mLoggerID, 10);
    zlog::IZLogManager::getRef().setIsDisplayOnConsole(mLoggerID, true);
    zlog::IZLogManager::getRef().setIsWriteToFile(mLoggerID, true);
    zlog::IZLogManager::getRef().setIsDisplayOnConsole(zlog::ZLOG_MAIN_LOGGER_ID, false);
    zlog::IZLogManager::getRef().setIsWriteToFile(zlog::ZLOG_MAIN_LOGGER_ID, false);

    //zlog::IZLogManager::getRef().configFromFile("./ZlogConfig.ini");
    //zlog::IZLogManager::getRef().updateConfig();
        
    std::stringstream oss;
    for (int i = 0; i < LOG_COLLECTION_TYPE_MAX; ++i)
    {
        oss.str("");
        oss << "Collection_Type_" << i;
        int logId = zlog::IZLogManager::getRef().createLogger(oss.str().c_str());
        zlog::IZLogManager::getRef().setLogFilePath(logId, "./log/");
        zlog::IZLogManager::getRef().setFilterLogLevel(logId, LOG_LEVEL_INFO);
        zlog::IZLogManager::getRef().setLogFileSizeLimit(logId, 8);
        zlog::IZLogManager::getRef().setIsDisplayOnConsole(logId, true);
        zlog::IZLogManager::getRef().setIsWriteToFile(logId, true);
        mLogCollections[i] = logId;
    }

    zlog::IZLogManager::getRef().start();
}

void LogUtil::Debug(const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_DEBUG)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_DEBUG, timestamp, msgBuffer);
    }
}

void LogUtil::Debug(CodeLocation location, const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_DEBUG)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_DEBUG, timestamp, location, msgBuffer);
    }
}

void LogUtil::Warn(const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_WARN)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_WARN, timestamp, msgBuffer);
    }
}

void LogUtil::Warn(CodeLocation location, const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_WARN)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_WARN, timestamp, location, msgBuffer);
    }
}

void LogUtil::Info(const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_INFO)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_INFO, timestamp, msgBuffer);
    }
}

void LogUtil::Info(CodeLocation location, const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_INFO)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_INFO, timestamp, location, msgBuffer);
    }
}

void LogUtil::Error(const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_ERROR)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_ERROR, timestamp, msgBuffer);
    }
}

void LogUtil::Error(CodeLocation location, const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_ERROR)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_ERROR, timestamp, location, msgBuffer);
    }
}

void LogUtil::Fatal(const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_FATAL)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_FATAL, timestamp, msgBuffer);
    }
}

void LogUtil::Fatal(CodeLocation location, const char *fmt, ...)
{
    if (mLevel <= LOG_LEVEL_FATAL)
    {
        std::string timestamp = GetSystemTimeString();
        char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
        DEAFAULT_LOG_FORMAT(msgBuffer, fmt, DEAFAULT_LOG_BUFFER_LENGTH - 1);
        Log(LOG_LEVEL_FATAL, timestamp, location, msgBuffer);
    }
}

/***********************************************************************
*    功能： 收集采样数据
*    参数： location 代码定位
*           collectType 指示收集数据类型，预留 LOG_COLLECTION_TYP_MAX 个
*           format 日志内容格式化串
************************************************************************/
void LogUtil::CollectData(CodeLocation location, LogCollectionType collectType, const char* format, ...)
{
    if (collectType < 0 || collectType >= LOG_COLLECTION_TYPE_MAX) return;

    char msgBuffer[DEAFAULT_LOG_BUFFER_LENGTH];
    DEAFAULT_LOG_FORMAT(msgBuffer, format, DEAFAULT_LOG_BUFFER_LENGTH - 1);

    std::string tempMsg(msgBuffer);
    tempMsg.append("  <-- ").append(location.GetFunctionNameFull());
    LOG_STREAM(mLogCollections[collectType], LOG_LEVEL_INFO, location.GetFileName().c_str(), location.GetLineNumber(), tempMsg);
}

void LogUtil::Log(LogLevelType level, std::string timestamp, std::string message)
{
    //if (mpLogCallback != nullptr)
    //{
    //    std::string strMessage = timestamp;
    //    std::string levelStr = GetLevelString(level);

    //    strMessage.append(" [" + levelStr + "] ")
    //        .append(message);

    //    mpLogCallback(strMessage.c_str());
    //}

    if (mpLogCallback != nullptr)
    {
        mpLogCallback(level, nullptr, message);
    }
}

void LogUtil::Log(LogLevelType level, std::string timestamp, CodeLocation location, std::string message)
{
    //if (mpLogCallback != nullptr)
    //{
    //    std::string strMessage = timestamp;
    //    std::string levelStr = GetLevelString(level);

    //    strMessage.append(" [" + levelStr + "] ")
    //        .append(message + " ")
    //        .append(location.ToString());
    //    mpLogCallback(strMessage.c_str());
    //}

    if (mpLogCallback != nullptr)
    {
        mpLogCallback(level, &location, message);
    }
}

void LogUtil::DefaultLogHandler(LogLevelType level, CodeLocation *pLocation, std::string& msg)
{
    if (mLoggerID == 0) return;

    std::string tempMsg = msg;
    std::string filename;
    int lineNum = 0;
    if (pLocation != nullptr)
    {
        tempMsg.append("  <-- ").append(pLocation->GetFunctionNameFull());
        filename = pLocation->GetFileName();
        lineNum = pLocation->GetLineNumber();
    }

    switch (level)
    {
    case LOG_LEVEL_TRACE:
        LOG_STREAM(mLoggerID, LOG_LEVEL_TRACE, filename.c_str(), lineNum, tempMsg);
        break;
    case LOG_LEVEL_DEBUG:
        LOG_STREAM(mLoggerID, LOG_LEVEL_DEBUG, filename.c_str(), lineNum, tempMsg);
        break;
    case LOG_LEVEL_INFO:
        LOG_STREAM(mLoggerID, LOG_LEVEL_INFO, filename.c_str(), lineNum, tempMsg);
        break;
    case LOG_LEVEL_WARN:
        LOG_STREAM(mLoggerID, LOG_LEVEL_WARN, filename.c_str(), lineNum, tempMsg);
        break;
    case LOG_LEVEL_ERROR:
        LOG_STREAM(mLoggerID, LOG_LEVEL_ERROR, filename.c_str(), lineNum, tempMsg);
        break;
    case LOG_LEVEL_ALARM:
        LOG_STREAM(mLoggerID, LOG_LEVEL_ALARM, filename.c_str(), lineNum, tempMsg);
        break;
    case LOG_LEVEL_FATAL:
        LOG_STREAM(mLoggerID, LOG_LEVEL_FATAL, filename.c_str(), lineNum, tempMsg);
        break;
    case LOG_LEVEL_NONE:
        break;
    default:
        break;
    }
}

std::string LogUtil::GetSystemTimeString()
{
    char timeBuffer[100] = { 0 };
#ifdef WIN32
    SYSTEMTIME curTime;
    GetLocalTime(&curTime);
    sprintf_s(timeBuffer, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                curTime.wYear, curTime.wMonth, curTime.wDay,
                curTime.wHour, curTime.wMinute, curTime.wSecond, curTime.wMilliseconds);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *pLocalTime = localtime(&tv.tv_sec);
    int milliSeconds = tv.tv_usec / 1000;
    sprintf_s(timeBuffer, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                pLocalTime->tm_year + 1990, pLocalTime->tm_mon + 1, pLocalTime->tm_mday,
                pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec, milliSeconds);
#endif

    return std::string(timeBuffer);
}

std::string LogUtil::GetLevelString(LogLevelType logLevel)
{
    std::string retValue = "UNKNOW";
    switch (logLevel)
    {
    case LOG_LEVEL_NONE:
        retValue = "NONE";
        break;
    case LOG_LEVEL_TRACE:
        retValue = "TRACE";
        break;
    case LOG_LEVEL_DEBUG:
        retValue = "DEBUG";
        break;
    case LOG_LEVEL_INFO:
        retValue = "INFO";
        break;
    case LOG_LEVEL_WARN:
        retValue = "WARN";
        break;
    case LOG_LEVEL_ERROR:
        retValue = "ERROR";
        break;
    case LOG_LEVEL_ALARM:
        retValue = "ALARM";
        break;
    case LOG_LEVEL_FATAL:
        retValue = "FATAL";
        break;
    default:
        break;
    }

    return retValue;
}

std::string LogUtil::ConvertToString(std::wstring wstring)
{
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> cvtAnsi(new std::codecvt<wchar_t, char, std::mbstate_t>("chs"));
    std::string retString = cvtAnsi.to_bytes(wstring);

    return retString;
}

CodeLocation::CodeLocation(const char* fileName, const char* funcName, int lineNumber)
{
    m_strFileName = fileName;
    m_strFuncName = funcName;
    m_nLineNumber = lineNumber;
}

CodeLocation::~CodeLocation()
{
    //std::cout << "CodeLocation::~CodeLocation()" << std::endl;
}

int CodeLocation::GetLineNumber()
{
    return m_nLineNumber;
}

std::string CodeLocation::GetFileName()
{
    return m_strFileName;
}

std::string CodeLocation::GetShortFileName()
{
    size_t index = std::string::npos;
    m_strShortFileName = m_strFileName;

    index = m_strFileName.find_last_of('\\');
    if (index != std::string::npos)
    {
        m_strShortFileName.erase(0, index + 1);
    }

    return m_strShortFileName;
}

std::string CodeLocation::GetShortClassName()
{
    std::string className("");
    size_t index = m_strFuncName.find_last_of(":");
    if (index != std::string::npos)
    {
        className = m_strFuncName.substr(0, index - 1);
    }
    index = className.find_last_of(":");
    if (index != std::string::npos)
    {
        className.erase(0, index + 1);
    }

    return className;
}

std::string CodeLocation::GetFunctionName()
{
    std::string funcName = m_strFuncName;
    size_t index = funcName.find_last_of(":");
    if (index != std::string::npos)
    {
        funcName.erase(0, index + 1);
    }

    return funcName;
}

std::string CodeLocation::GetFunctionNameFull()
{
    return m_strFuncName;
}

std::string CodeLocation::ToString()
{
    std::ostringstream oss;
    oss << m_nLineNumber;
    std::string retString = "[ ";
    retString.append(m_strFuncName);
    retString.append(" ");
    retString.append(GetShortFileName());
    retString.append(":");
    retString.append(oss.str());
    retString.append(" ]");

    return retString;
}
