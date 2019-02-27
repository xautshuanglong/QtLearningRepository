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
    static void SetPrivateKeyFile(const QString& privateKeyFile);
    static void SetCertificateFile(const QString& certificate);
    static void AddTrustedCertificateFile(const QString& trustedCertificate);
    static const OFString& GetPrivateKeyFile() { return mgDicomClientPrivateKey; }
    static const OFString& GetCertificateFile() { return mgDicomClientCertificate; }
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
    static OFString mgDicomClientPrivateKey;
    static OFString mgDicomClientCertificate;
    static OFList<OFString> mgTrustedCertificates;
};
