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

    void SetConnectionTimeout(int timeoutSeconds); // ���ó�ʱʱ�䣬�ڳ�ʼ������ǰ������Ч
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
    DcmTLSTransportLayer *mpTLSLayer;       // TLS ��ȫ���䣬�����֤
    int                   mTimeoutSeconds;  // ���Ӷ�ȡ��ʱ Ĭ�� 30S
    QString               mAppEntityTitle;  // ��Ӧ��ʵ�� Title
    QString               mPeerEntityTitle; // �Զ�Ӧ��ʵ�� Title
    QString               mServerIP;
    unsigned int          mServerPort;
    unsigned int          mMaxReceivePDU;
    bool                  mBlockingMode;    // true�������������ģʽ
};
