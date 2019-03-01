#pragma once

#include <QObject>
#include "IDicomSCUObserver.h"

class DicomSCUEcho;
class DicomSCUFind;
class DicomSCUGet;
class DicomSCUMove;
class DicomSCUStore;

class QTDICOMLIB_EXPORT DicomClient : public QObject, IDicomSCUObserver
{
    Q_OBJECT
public:
    explicit DicomClient(QObject *parent = 0);
    ~DicomClient();

    void SetConnectionTimeout(int timeoutSeconds) { m_connectTimeoutSec = timeoutSeconds; }
    void SetDIMSETimeout(int timeoutSeconds) { m_dimseTimeoutSeconds = timeoutSeconds; }
    void SetACSETimeout(int timeoutSeconds) { m_acseTimeoutSeconds = timeoutSeconds; }
    void SetPeerHostName(const QString& peerHostName) { m_peerHostName = peerHostName; }
    void SetPeerPort(const unsigned short peerPort) { m_peerPort = peerPort; }
    void SetPeerAETitle(const QString& peerAETitle) { m_peerAETitle = peerAETitle; }
    void SetAppAETitle(const QString& appAETitle) { m_appAETitle = appAETitle; }
    void SetDestinationTitle(const QString& destAETitle) { m_destAETitle = destAETitle; }
    void SetMaxReceivePDULength(const unsigned int& maxPDU) { m_maxReceivePDU = maxPDU; }
    void SetDIMSEBlockingMode(const bool blockFlag) { m_blockFlag = blockFlag; }

    void PerformEcho();   // 回显/确认
    void PerformFind();   // 查询/检索
    void PerformGet();    // 获取/下载
    void PerformMove();   // 移动/转储
    void PerformStore();  // 存储/上传

    // DICOM 响应处理（实现回调接口）
    virtual void HandleResponseEcho() override;
    virtual void HandleResponseFind() override;
    virtual void HandleResponseGet() override;
    virtual void HandleResponseMove() override;
    virtual void HandleResponseStore() override;

private:
    void RegisterObserver();

signals:
    void SigErrorString(const QString& errorString);
    void SigResponseEcho();
    void SigResponseFind();
    void SigResponseGet();
    void SigResponseMove();
    void SigResponseStore();

    public slots:

private:
    bool            m_blockFlag;
    int             m_connectTimeoutSec;   // 连接 SCP 超时
    int             m_dimseTimeoutSeconds; // 接收 DIMSE 消息超时  DIMSE：DICOM Messsage Service Element
    int             m_acseTimeoutSeconds;  // 接收 ACSE 消息超时   ACSE：Association Control Service Element
    unsigned short  m_peerPort;
    unsigned int    m_maxReceivePDU;       // 最大接收协议数据单元
    QString         m_peerHostName;
    QString         m_peerAETitle;         // 服务端（被请求端）Title
    QString         m_appAETitle;          // 当前应用的 Title
    QString         m_destAETitle;         // 目的端 Title， C-MOVE 可以转储给自己或其他应用实体。
    DicomSCUEcho   *m_pDicomEcho;
    DicomSCUFind   *m_pDicomFind;
    DicomSCUGet    *m_pDicomGet;
    DicomSCUMove   *m_pDicomMove;
    DicomSCUStore  *m_pDicomStore;
};
