#pragma once

#include "qtdicomlib_global.h"
#include <QObject>

class DcmFindSCU;
class DcmTLSTransportLayer;

class QTDICOMLIB_EXPORT DicomFindSCU : public QObject
{
    Q_OBJECT

public:
    DicomFindSCU(QObject *parent = Q_NULLPTR);
    ~DicomFindSCU();

    void SetTimeout(int timeoutSeconds) { mTimeoutSeconds = timeoutSeconds; } // 设置超时时间，在初始化网络前设置有效
    void SetServerIpPort(const QString& serverIP, const unsigned int& serverPort);
    void SetAppEntityTitle(const QString& selfTitle, const QString& peerTitle);
    void SetOutputDirectory(const QString& outDirectory) { mOutputDirectory = outDirectory; }
    void SetOutputXmlFilename(const QString& outXmlFilename) { mOutputXmlFilename = outXmlFilename; }
    void SetMaxReceivePDU(const unsigned int& maxPDU) { mMaxReceivePDU = maxPDU; }
    void AppendOverrideKey(const QString& key) { mOverrideKes.append(key); }
    void ClearOverridKeys() { mOverrideKes.clear(); }
    void AppendFilename(const QString& filename) { mFilenameList.append(filename); }
    void ClearFilenames() { mFilenameList.clear(); }
    void InitializeNetwork();
    void DropNetwork();
    void PerformQuery();

signals:
    void SigErrorString(const QString & errorString);

private:
    DcmFindSCU           *mpDcmFindSCU;
    DcmTLSTransportLayer *mpTLSLayer;       // TLS 安全传输，身份认证
    int                   mTimeoutSeconds;  // 连接读取超时 默认 30S
    QString               mAppEntityTitle;  // 本应用实体 Title
    QString               mPeerEntityTitle; // 对端应用实体 Title
    QString               mServerIP;
    unsigned int          mServerPort;
    unsigned int          mMaxReceivePDU;
    QStringList           mOverrideKes;     // list of keys/paths that override those in the query files
    QStringList           mFilenameList;
    QString               mOutputDirectory;
    QString               mOutputXmlFilename;
};
