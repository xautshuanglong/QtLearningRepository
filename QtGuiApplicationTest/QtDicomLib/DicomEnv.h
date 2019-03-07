#pragma once

#include "qtdicomlib_global.h"

// DCMTK Headers
#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/ofstd/oflist.h>

// Self Headers
#include <LogUtil.h>

class QTDICOMLIB_EXPORT DicomEnv
{
public:
    ~DicomEnv();
    static void Initialize(LogCallback pLogCallback = nullptr);
    static void Uninitialize();

    // SSL 证书验证所需参数
    static void SetPrivateKeyPasswd(const QString& privateKeyPasswd);
    static void SetPrivateKeyFile(const QString& privateKeyFile);
    static void SetCertificateFile(const QString& certificate);
    static void AddTrustedCertificateFile(const QString& trustedCertificate);
    static const OFString& GetPrivateKeyPasswd() { return mgDicomPrivateKeyPasswd; }
    static const OFString& GetPrivateKeyFile() { return mgDicomPrivateKeyFile; }
    static const OFString& GetCertificateFile() { return mgDicomCertificateFile; }
    static OFList<OFString> GetTrustedCertificateList() { return mgTrustedCertificates; }

private:
    DicomEnv();
    static void InitializeNetwork();
    static void UninitializeNetwork();
    static void InitializeLog4Cplus();
    static void UninitializeLog4Cplus();
    static void ConfigureLog4CplusFormFile(); // 通过文件配置日志工具，方便修改等级，添加日志策略及过滤器
    static void ConfigureLog4CplusFormCode(); // 硬编码配置，防止用户篡改日志工具配置文件

private:
    static bool isNetworkInitialized;
    static bool isLog4CplusInitialized;
    static OFString mgDicomPrivateKeyPasswd;
    static OFString mgDicomPrivateKeyFile;
    static OFString mgDicomCertificateFile;
    static OFList<OFString> mgTrustedCertificates;
};
