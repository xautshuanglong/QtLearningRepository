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

// SSL 证书验证
OFString DicomEnv::mgDicomPrivateKeyPasswd;       // DICOM 客户端秘钥短语
OFString DicomEnv::mgDicomPrivateKeyFile;         // DICOM 客户端秘钥路径
OFString DicomEnv::mgDicomCertificateFile;        // DICOM 客户端证书路径
OFList<OFString> DicomEnv::mgTrustedCertificates; // DICOM 客户端信任证书列表

DicomEnv::DicomEnv()
{
}

DicomEnv::~DicomEnv()
{
}

void DicomEnv::Initialize(LogCallback pLogCallback /* = nullptr */)
{
#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG, pLogCallback);
#else
    LogUtil::Init(LOG_LEVEL_INFO, pLogCallback);
#endif

    DicomEnv::InitializeLog4Cplus();
    DicomEnv::InitializeNetwork();
}

void DicomEnv::Uninitialize()
{
    DicomEnv::UninitializeNetwork();
    DicomEnv::UninitializeLog4Cplus();
}

void DicomEnv::InitializeNetwork()
{
    if (!isNetworkInitialized)
    {
        isNetworkInitialized = true;
        OFStandard::initializeNetwork(); // 初始化 DICOM 网络环境
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

void DicomEnv::UninitializeLog4Cplus()
{
    dcmtk::log4cplus::Logger::shutdown();
}

// 模拟 DCMTK 命令行工具参数设置配置文件路径，进而配置日志输出策略。
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

    // 重新配置 DCMTK Log4Cplus
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

void DicomEnv::SetPrivateKeyPasswd(const QString& privateKeyPasswd)
{
    mgDicomPrivateKeyPasswd.assign(privateKeyPasswd.toStdString().c_str());
}

void DicomEnv::SetPrivateKeyFile(const QString& privateKeyFile)
{
    QString appDirPath = QCoreApplication::applicationDirPath();
    QDir filePath(privateKeyFile);
    if (filePath.isAbsolute())
    {
        mgDicomPrivateKeyFile.assign(privateKeyFile.toStdString().c_str());
    }
    else
    {
        QString absoluteFile = appDirPath + "\\" + privateKeyFile;
        mgDicomPrivateKeyFile.assign(absoluteFile.toStdString().c_str());
    }

}

void DicomEnv::SetCertificateFile(const QString& certificate)
{
    QString appDirPath = QCoreApplication::applicationDirPath();
    QDir filePath(certificate);
    if (filePath.isAbsolute())
    {
        mgDicomCertificateFile.assign(certificate.toStdString().c_str());
    }
    else
    {
        QString absoluteFile = appDirPath + "\\" + certificate;
        mgDicomCertificateFile.assign(absoluteFile.toStdString().c_str());
    }
}

void DicomEnv::AddTrustedCertificateFile(const QString& trustedCertificate)
{
    QString appDirPath = QCoreApplication::applicationDirPath();
    QDir filePath(trustedCertificate);
    if (filePath.isAbsolute())
    {
        mgTrustedCertificates.push_back(trustedCertificate.toStdString().c_str());
    }
    else
    {
        QString absoluteFile = appDirPath + "\\" + trustedCertificate;
        mgTrustedCertificates.push_back(absoluteFile.toStdString().c_str());
    }
}
