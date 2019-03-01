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

    void PerformEcho();   // ����/ȷ��
    void PerformFind();   // ��ѯ/����
    void PerformGet();    // ��ȡ/����
    void PerformMove();   // �ƶ�/ת��
    void PerformStore();  // �洢/�ϴ�

    // DICOM ��Ӧ����ʵ�ֻص��ӿڣ�
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
    int             m_connectTimeoutSec;   // ���� SCP ��ʱ
    int             m_dimseTimeoutSeconds; // ���� DIMSE ��Ϣ��ʱ  DIMSE��DICOM Messsage Service Element
    int             m_acseTimeoutSeconds;  // ���� ACSE ��Ϣ��ʱ   ACSE��Association Control Service Element
    unsigned short  m_peerPort;
    unsigned int    m_maxReceivePDU;       // ������Э�����ݵ�Ԫ
    QString         m_peerHostName;
    QString         m_peerAETitle;         // ����ˣ�������ˣ�Title
    QString         m_appAETitle;          // ��ǰӦ�õ� Title
    QString         m_destAETitle;         // Ŀ�Ķ� Title�� C-MOVE ����ת�����Լ�������Ӧ��ʵ�塣
    DicomSCUEcho   *m_pDicomEcho;
    DicomSCUFind   *m_pDicomFind;
    DicomSCUGet    *m_pDicomGet;
    DicomSCUMove   *m_pDicomMove;
    DicomSCUStore  *m_pDicomStore;
};
