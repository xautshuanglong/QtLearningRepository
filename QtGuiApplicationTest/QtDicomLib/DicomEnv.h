#pragma once

#include "qtdicomlib_global.h"

// DCMTK Headers
#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/ofstd/oflist.h>

class QTDICOMLIB_EXPORT DicomEnv
{
public:
    ~DicomEnv();
    static void Initialize();
    static void Uninitialize();

    // SSL ֤����֤�������
    static void SetPrivateKeyPasswd(const QString& privateKeyPasswd);
    static void SetPrivateKeyFile(const QString& privateKeyFile);
    static void SetCertificateFile(const QString& certificate);
    static void AddTrustedCertificateFile(const QString& trustedCertificate);
    static const OFString& GetPrivateKeyPasswd() { return mgDicomPrivateKeyPasswd; }
    static const OFString& GetPrivateKeyFile() { return mgDicomPrivateKeyFile; }
    static const OFString& GetCertificateFile() { return mgDicomCertificateFile; }
    static OFList<OFString> GetTrustedCertificateList() { return mgTrustedCertificates; };

private:
    DicomEnv();
    static void InitializeNetwork();
    static void UninitializeNetwork();
    static void InitializeLog4Cplus();
    static void ConfigureLog4CplusFormFile(); // ͨ���ļ�������־���ߣ������޸ĵȼ��������־���Լ�������
    static void ConfigureLog4CplusFormCode(); // Ӳ�������ã���ֹ�û��۸���־���������ļ�

private:
    static bool isNetworkInitialized;
    static bool isLog4CplusInitialized;
    static OFString mgDicomPrivateKeyPasswd;
    static OFString mgDicomPrivateKeyFile;
    static OFString mgDicomCertificateFile;
    static OFList<OFString> mgTrustedCertificates;
};
