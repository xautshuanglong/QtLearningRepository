#pragma once

#include "qtdicomlib_global.h"
#include <QObject>

// DCMTK Headers
#include <dcmtk/dcmnet/dfindscu.h>

class DcmFindSCU;
class DcmTLSTransportLayer;

class QTDICOMLIB_EXPORT DicomFindSCU : public QObject, public DcmFindSCUCallback
{
    Q_OBJECT

public:
    DicomFindSCU(QObject *parent = Q_NULLPTR);
    ~DicomFindSCU();

    void SetTimeout(int timeoutSeconds) { mTimeoutSeconds = timeoutSeconds; } // ���ó�ʱʱ�䣬�ڳ�ʼ������ǰ������Ч
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

    // DcmFindSCUCallback handle find response
    virtual void callback(T_DIMSE_C_FindRQ *request, int responseCount, T_DIMSE_C_FindRSP *rsp, DcmDataset *responseIdentifiers) override;

private:
    void ExtractResponseToFile(int responseCount, DcmDataset *pResponseIdentifiers);

signals:
    void SigErrorString(const QString & errorString);

private:
    DcmFindSCU           *mpDcmFindSCU;
    DcmTLSTransportLayer *mpTLSLayer;       // TLS ��ȫ���䣬�����֤
    DcmFindSCUExtractMode mExtractMode;     // ����Ӧ�����ȡģʽ
    int                   mTimeoutSeconds;  // ���Ӷ�ȡ��ʱ Ĭ�� 30S
    QString               mAppEntityTitle;  // ��Ӧ��ʵ�� Title
    QString               mPeerEntityTitle; // �Զ�Ӧ��ʵ�� Title
    QString               mServerIP;
    unsigned int          mServerPort;
    unsigned int          mMaxReceivePDU;
    QStringList           mOverrideKes;     // list of keys/paths that override those in the query files
    QStringList           mFilenameList;
    QString               mOutputDirectory;
    QString               mOutputXmlFilename;
};
