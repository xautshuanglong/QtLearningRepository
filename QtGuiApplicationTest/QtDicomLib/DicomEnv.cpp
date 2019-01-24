#include "DicomEnv.h"

// QT Headers
#include <QDir>
#include <QString>
#include <QCoreApplication>

// DCMTK Headers
#include <dcmtk/oflog/consap.h>
#include <dcmtk/oflog/fileap.h>
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/ofstd/ofstd.h>
#include <dcmtk/ofstd/ofutil.h>
#include <dcmtk/ofstd/ofconapp.h>
#include <dcmtk/ofstd/ofcmdln.h>

// Self Headers
#include <LogUtil.h>

bool DicomEnv::isNetworkInitialized = false;
bool DicomEnv::isLog4CplusInitialized = false;

DicomEnv::DicomEnv()
{
}

DicomEnv::~DicomEnv()
{
}

void DicomEnv::Initialize()
{
#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG);
#else
    LogUtil::Init(LOG_LEVEL_INFO);
#endif

    DicomEnv::InitializeLog4Cplus();
    DicomEnv::InitializeNetwork();
}

void DicomEnv::Uninitialize()
{
    DicomEnv::UninitializeNetwork();
}

void DicomEnv::InitializeNetwork()
{
    if (!isNetworkInitialized)
    {
        isNetworkInitialized = true;
        OFStandard::initializeNetwork(); // ��ʼ�� DICOM ���绷��
    }
}

void DicomEnv::UninitializeNetwork()
{
    if (isNetworkInitialized)
    {
        isNetworkInitialized = false;
        OFStandard::shutdownNetwork();
    }
}

void DicomEnv::InitializeLog4Cplus()
{
    if (isLog4CplusInitialized) return;
    isLog4CplusInitialized = true;

    //DicomEnv::ConfigureLog4CplusFormFile();
    DicomEnv::ConfigureLog4CplusFormCode();
}

// ģ�� DCMTK �����й��߲������������ļ�·��������������־������ԡ�
void DicomEnv::ConfigureLog4CplusFormFile()
{
    QString appName = QCoreApplication::applicationName();
    QString appFilePath = QCoreApplication::applicationFilePath();
    QString appDirPath = QCoreApplication::applicationDirPath();
    OFConsoleApplication dcmConsoleApp("QtDemo_dcmtk");
    OFCommandLine cmd;
    OFLog::addOptions(cmd);

    char tempFilePath[MAX_PATH] = { 0 };
    std::memcpy(tempFilePath, appFilePath.toStdString().c_str(), appFilePath.length());
    char tempConfigPath[MAX_PATH] = { 0 };
    appDirPath.append("/config/log4cplus.properties");
    std::memcpy(tempConfigPath, appDirPath.toStdString().c_str(), appDirPath.length());
    char *argValues[] =
    {
        tempFilePath,
        "-lc", // "--log-config",
        tempConfigPath
    };
    int argc = sizeof(argValues) / sizeof(char*);
    if (!OFStandard::fileExists(tempConfigPath))
    {
        LogUtil::Warn(CODE_LOCATION, "File does not exist: %s", tempConfigPath);
    }
    else if (!OFStandard::isReadable(tempConfigPath))
    {
        LogUtil::Warn(CODE_LOCATION, "File is not readable: %s", tempConfigPath);
    }
    else
    {
        dcmConsoleApp.parseCommandLine(cmd, argc, argValues);
        OFLog::configureFromCommandLine(cmd, dcmConsoleApp);
    }
}

void DicomEnv::ConfigureLog4CplusFormCode()
{
    QString appDirPath = QCoreApplication::applicationDirPath();
    QString curDirPath = QCoreApplication::applicationDirPath();
    QString logsDirPath = "./logs"; // appDirPath.append("/logs");
    QDir logsDir(logsDirPath);
    if (!logsDir.exists())
    {
        logsDir.mkpath(logsDirPath);
    }

    QString tempLogFilename = logsDirPath + "/DCMTK_TempFile.log";
    QString rollingLogFilename = logsDirPath + "/DCMTK_RollingFile.log";

    const char *noDatePattern = "%D{%H:%M:%S.%q} %-5p %c{2} %x - %m  --> %l%n";
    const char *rollingFilePattern = "%D{%Y-%m-%d %H:%M:%S.%q} %-5p %c{2} %x - %m  --> %l%n";

    OFunique_ptr<dcmtk::log4cplus::Layout> consoleLayout(new dcmtk::log4cplus::PatternLayout(noDatePattern));
    OFunique_ptr<dcmtk::log4cplus::Layout> tempFileLayout(new dcmtk::log4cplus::PatternLayout(noDatePattern));
    OFunique_ptr<dcmtk::log4cplus::Layout> rollingFileLayout(new dcmtk::log4cplus::PatternLayout(rollingFilePattern));

    dcmtk::log4cplus::SharedAppenderPtr console(
        new dcmtk::log4cplus::ConsoleAppender(OFTrue, OFTrue));
    console->setLayout(OFmove(consoleLayout));

    dcmtk::log4cplus::SharedAppenderPtr tempFile(
        new dcmtk::log4cplus::FileAppender(tempLogFilename.toStdString().c_str()));
    tempFile->setLayout(OFmove(tempFileLayout));

    dcmtk::log4cplus::SharedAppenderPtr rollingFile(
        new dcmtk::log4cplus::RollingFileAppender(rollingLogFilename.toStdString().c_str(), 10*1024*1024, 100));
    rollingFile->setLayout(OFmove(rollingFileLayout));

    // �������� DCMTK Log4Cplus
    dcmtk::log4cplus::Logger rootLogget = dcmtk::log4cplus::Logger::getRoot();
    rootLogget.removeAllAppenders();
    rootLogget.addAppender(console);
    rootLogget.addAppender(tempFile);
    rootLogget.addAppender(rollingFile);
#ifdef _DEBUG
    rootLogget.setLogLevel(OFLogger::DEBUG_LOG_LEVEL);
#else
    rootLogget.setLogLevel(OFLogger::INFO_LOG_LEVEL);
#endif
}
