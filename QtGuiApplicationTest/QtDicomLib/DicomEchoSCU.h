#pragma once

#include "qtdicomlib_global.h"
#include "qobject.h"

class DcmSCU;
class DcmTLSTransportLayer;

class QTDICOMLIB_EXPORT DicomEchoSCU : public QObject
{
    Q_OBJECT
public:
    DicomEchoSCU(QObject *parent = Q_NULLPTR);
    ~DicomEchoSCU();

    void SetConnectionTimeout(int timeoutSeconds); // 设置超时时间，在初始化网络前设置有效
    void SetDIMSETimeout(int timeoutSeconds);
    void SetACSETimeout(int timeoutSeconds);
    void SetPeerHostName(const QString& peerHostName);
    void SetPeerPort(const unsigned short peerPort);
    void SetPeerAETitle(const QString& peerAETitle);
    void SetAppAETitle(const QString& appAETitle);
    void SetMaxReceivePDULength(const unsigned int& maxPDU);
    void SetDIMSEBlockingMode(const bool blockFlag);
    void PerformEcho();

private:
    DcmSCU               *mpDcmSCU;
    DcmTLSTransportLayer *mpTLSLayer;       // TLS 安全传输，身份认证
    int                   mTimeoutSeconds;  // 连接读取超时 默认 30S
    QString               mAppEntityTitle;  // 本应用实体 Title
    QString               mPeerEntityTitle; // 对端应用实体 Title
    QString               mServerIP;
    unsigned int          mServerPort;
    unsigned int          mMaxReceivePDU;
    bool                  mBlockingMode;    // true：网络采用阻塞模式
};
