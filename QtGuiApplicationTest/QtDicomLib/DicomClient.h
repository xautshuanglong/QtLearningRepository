#pragma once

#include <QObject>
#include "IDicomSCUObserver.h"

class DicomSCUEcho;
class DicomSCUFind;
class DicomSCUGet;
class DicomSCUMove;
class DicomSCUStore;
class DicomExecutor;

class QTDICOMLIB_EXPORT DicomClient : public QObject, IDicomSCUObserver
{
    Q_OBJECT
public:
    explicit DicomClient(QObject *parent = 0);
    ~DicomClient();

    void SetConnectionTimeout(int timeoutSeconds);
    void SetDIMSETimeout(int timeoutSeconds);
    void SetACSETimeout(int timeoutSeconds);
    void SetPeerHostName(const QString& peerHostName);
    void SetPeerPort(const unsigned short peerPort);
    void SetPeerAETitle(const QString& peerAETitle);
    void SetAppAETitle(const QString& appAETitle);
    void SetMaxReceivePDULength(const unsigned int& maxPDU);
    void SetDIMSEBlockingMode(const bool blockFlag);

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
    DicomExecutor  *m_pDicomExecutor;
};
