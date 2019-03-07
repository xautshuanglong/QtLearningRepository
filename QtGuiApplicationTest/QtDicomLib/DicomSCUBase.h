#pragma once

#include "qtdicomlib_global.h"

// QT Headers
#include <QSharedPointer>

// DCMTK Headers
#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/dcmnet/dimse.h>

// Self Headers
#include "DicomTaskData.h"

class IDicomSCUObserver;

struct DicomPresentationContext
{
    OFString abstractSyntaxName;
    OFList<OFString> transferSyntaxes;
    T_ASC_SC_ROLE roleSelect;
};

typedef void(*DIMSE_GetUserCallbackEx)(void *callbackData, T_DIMSE_C_GetRQ *request, int responseCount, T_DIMSE_C_GetRSP *response, OFBool &continueSession);
typedef void(*DIMSE_SubOpProviderCallbackEx)(void *subOpCallbackData, T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID *presentationID,
                                             OFBool &continueSession, Uint16 &cstoreReturnStatus, DcmDataset *pRspDataset);

class DicomSCUBase
{
public:
    DicomSCUBase();
    ~DicomSCUBase();
    void RegisterObserver(IDicomSCUObserver* pDicomObserver = Q_NULLPTR);
    void AddPresentationContext(const OFString &abstractSyntax, const OFList<OFString> &transferSyntaxes, const T_ASC_SC_ROLE role = ASC_SC_ROLE_DEFAULT);
    void ClearPresentationContex();
    void AddQueryKey(const OFString& queryKey) { m_overrideKeys.push_back(queryKey); }
    void ClearQueryKey() { m_overrideKeys.clear(); }

    void SetTimeoutConnection(const Uint32 timeoutSecondes) { m_connectTimeoutSeconds = timeoutSecondes; }
    void SetTimeoutDimse(const Uint32 timeoutSecondes) { m_dimseTimeoutSeconds = timeoutSecondes; }
    void SetTimeoutAcse(const Uint32 timeoutSecondes) { m_acseTimeoutSeconds = timeoutSecondes; }
    void SetMaxReceivePDU(const Uint32 maxPDU) { m_maxReceivePDU = maxPDU; }
    void SetPeerPort(const Uint16 peerPort) { m_peerPort = peerPort; }
    void SetPeerHostname(const OFString& peerHostname) { m_peerHostName = peerHostname; }
    void SetAppTitle(const OFString& appTitle) { m_appAETitle = appTitle; }
    void SetPeerTitle(const OFString& peerTitle) { m_peerAETitle = peerTitle; }
    void SetDestinationTitle(const OFString& destTitle) { m_destAETitle = destTitle; }

    OFCondition InitNetwork();
    OFCondition UseSecureConnection();
    OFCondition NegotiateAssociation();
    OFCondition ReleaseAssociation();
    OFCondition AbortAssociation();
    OFCondition DropNetwork();
    OFBool IsConnected();

    virtual OFCondition ExcuteOperation(QSharedPointer<DicomTaskBase> &pDicomTask) = 0;

protected:
    OFCondition EchoUser();
    OFCondition FindUser(const char *abstractSyntax, OFList<OFString> *pOverrideKeys,
                         T_DIMSE_C_FindRQ *pRequest, T_DIMSE_C_FindRSP *pResponse,
                         DIMSE_FindUserCallback callback = Q_NULLPTR, void *callbackData = Q_NULLPTR);
    OFCondition GetUser(const char *abstractSyntax, OFList<OFString> *pOverrideKeys,
                        T_DIMSE_C_GetRQ *pRequest, T_DIMSE_C_GetRSP *pResponse,
                        DIMSE_GetUserCallbackEx callback, void *callbackData,
                        DIMSE_SubOpProviderCallbackEx subOpCallback, void *subOpCallbackData);
    OFCondition MoveUser(const char *abstractSyntax, OFList<OFString> *pOverrideKeys,
                         T_DIMSE_C_MoveRQ *pRequest, T_DIMSE_C_MoveRSP *pResponse,
                         DIMSE_MoveUserCallback callback, void *callbackData,
                         DIMSE_SubOpProviderCallback subOpCallback, void *subOpCallbackData);
    OFCondition StoreUser(DcmDataset *pStoreDataset,
                          T_DIMSE_C_StoreRQ *pRequest, T_DIMSE_C_StoreRSP *pResponse,
                          DIMSE_StoreUserCallback callback, void *callbackData);

private:
    // DCMTK 中的 DIMSE_getUserEx 未经测试，存在缺陷，此处仿照 scu.cpp 重新实现 C-GET 操作。
    OFCondition DIMSE_getUserEx(T_ASC_Association *pAssociation, T_ASC_PresentationContextID presentationID, T_DIMSE_C_GetRQ *pRequest,
                                DcmDataset *pRequestIdentifiers, DIMSE_GetUserCallbackEx callback, void *callbackData,
                                T_DIMSE_BlockingMode blockMode, int timeout, T_ASC_Network *pNetwork,
                                DIMSE_SubOpProviderCallbackEx subOpCallback, void *pSubOpCallbackData,
                                T_DIMSE_C_GetRSP *pResponse, DcmDataset **ppStatusDetail, DcmDataset **ppResponseIdentifers);

protected:
    T_ASC_Association               *m_pAssociation;
    T_ASC_Network                   *m_pNetwork;
    T_ASC_Parameters                *m_pParameters;
    T_DIMSE_BlockingMode             m_blockMode;
    OFList<DicomPresentationContext> m_presentationContexts;
    OFList<OFString>                 m_overrideKeys;

    IDicomSCUObserver *m_pDicomObserver;
    Uint32             m_connectTimeoutSeconds;   // 连接 SCP 超时
    Uint32             m_dimseTimeoutSeconds;     // 接收 DIMSE 消息超时  DIMSE：DICOM Messsage Service Element
    Uint32             m_acseTimeoutSeconds;      // 接收 ACSE 消息超时   ACSE：Association Control Service Element
    Uint16             m_peerPort;
    Uint32             m_maxReceivePDU;           // 最大接收协议数据单元
    OFString           m_peerHostName;
    OFString           m_peerAETitle;
    OFString           m_appAETitle;
    OFString           m_destAETitle;
};
