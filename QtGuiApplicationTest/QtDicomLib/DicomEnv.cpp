#include "DicomEnv.h"

// QT Headers
#include <QString>
#include <QCoreApplication>

// DCMTK Headers
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/ofstd/ofstd.h>
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
