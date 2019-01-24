#pragma once

#include "qtdicomlib_global.h"

// DCMTK Headers
#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/dcmnet/dimse.h>

class IDicomSCUObserver;

struct DicomPresentationContext
{
    OFString abstractSyntaxName;
    OFList<OFString> transferSyntaxes;
    T_ASC_SC_ROLE roleSelect;
};

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

    OFCondition InitNetwork();
    OFCondition UseSecureConnection(DcmTransportLayer *transLayer);
    OFCondition NegotiateAssociation();
    OFCondition ReleaseAssociation();
    OFCondition AbortAssociation();
    OFCondition DropNetwork();
    OFBool IsConnected();

    OFCondition EchoUser();
    OFCondition FindUser(const char *abstractSyntax, OFList<OFString> *pOverrideKeys,
                         T_DIMSE_C_FindRQ *pRequest, T_DIMSE_C_FindRSP *pRsponse,
                         DIMSE_FindUserCallback callback = Q_NULLPTR, void *callbackData = Q_NULLPTR);
    OFCondition GetUser(const char *abstractSyntax, OFList<OFString> *pOverrideKeys,
                        T_DIMSE_C_GetRQ *pRequest, T_DIMSE_C_GetRSP *pRsponse,
                        DIMSE_GetUserCallback callback, void *callbackData,
                        DIMSE_SubOpProviderCallback subOpCallback, void *subOpCallbackData);
    OFCondition MoveUser();
    OFCondition StoreUser();

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
};
