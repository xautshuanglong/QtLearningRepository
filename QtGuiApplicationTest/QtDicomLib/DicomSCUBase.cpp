#include "DicomSCUBase.h"

#include "IDicomSCUObserver.h"
#include "DicomEnv.h"

// DCMTK Headers
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcpath.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmnet/cond.h>
#include <dcmtk/dcmtls/tlslayer.h>

DicomSCUBase::DicomSCUBase()
    : m_pAssociation(Q_NULLPTR)
    , m_pNetwork(Q_NULLPTR)
    , m_pParameters(Q_NULLPTR)
    , m_pDicomObserver(Q_NULLPTR)
    , m_blockMode(DIMSE_NONBLOCKING)
    , m_connectTimeoutSeconds(30)
    , m_dimseTimeoutSeconds(10)
    , m_acseTimeoutSeconds(30)
    , m_peerPort(5678)
    , m_maxReceivePDU(32768)
    , m_peerHostName("localhost")
    , m_peerAETitle("MGIUSDICOM")
    , m_appAETitle("DebugTest")
    , m_destAETitle("DebugTest")
{
}

DicomSCUBase::~DicomSCUBase()
{
    this->DropNetwork();
}

void DicomSCUBase::RegisterObserver(IDicomSCUObserver* pDicomObserver)
{
    m_pDicomObserver = pDicomObserver;
}

void DicomSCUBase::AddPresentationContext(const OFString &abstractSyntax, const OFList<OFString> &transferSyntaxes, const T_ASC_SC_ROLE role /* = ASC_SC_ROLE_DEFAULT */)
{
    DicomPresentationContext presentationContext;
    presentationContext.abstractSyntaxName = abstractSyntax;
    OFListConstIterator(OFString) it = transferSyntaxes.begin();
    OFListConstIterator(OFString) endOfList = transferSyntaxes.end();
    while (it != endOfList)
    {
        presentationContext.transferSyntaxes.push_back(*it);
        it++;
    }
    presentationContext.roleSelect = role;
    m_presentationContexts.push_back(presentationContext);
}

void DicomSCUBase::ClearPresentationContex()
{
    m_presentationContexts.clear();
}

OFCondition DicomSCUBase::InitNetwork()
{
    if (this->IsConnected())
    {
        return NET_EC_AlreadyConnected;
    }
    this->DropNetwork();

    OFString tempStr;
    OFCondition condition = ASC_initializeNetwork(NET_REQUESTOR, 0, m_acseTimeoutSeconds, &m_pNetwork);
    if (condition.bad())
    {
        DCMNET_ERROR(DimseCondition::dump(tempStr, condition));
        return condition;
    }

    condition = ASC_createAssociationParameters(&m_pParameters, m_maxReceivePDU);
    if (condition.bad())
    {
        DCMNET_ERROR(DimseCondition::dump(tempStr, condition));
        return condition;
    }

    condition = ASC_setAPTitles(m_pParameters, m_appAETitle.c_str(), m_peerAETitle.c_str(), NULL);
    if (condition.bad())
    {
        DCMNET_ERROR(DimseCondition::dump(tempStr, condition));
        return condition;
    }

    DIC_NODENAME tempPeerHost;
    const OFString tempLocalHost = OFStandard::getHostName();
    sprintf(tempPeerHost, "%s:%d", m_peerHostName.c_str(), OFstatic_cast(int, m_peerPort));
    condition = ASC_setPresentationAddresses(m_pParameters, tempLocalHost.c_str(), tempPeerHost);
    if (condition.bad())
    {
        DCMNET_ERROR(DimseCondition::dump(tempStr, condition));
        return condition;
    }

    Uint32 nextFreePresID = 257;
    Uint32 numContexts = ASC_countPresentationContexts(m_pParameters);
    if (numContexts <= 127)
    {
        nextFreePresID = 2 * numContexts + 1; /* add 1 to point to the next free ID*/
    }
    if ((numContexts + m_presentationContexts.size()) > 128)
    {
        DCMNET_WARN("Number of presentation contexts exceeds 128 (" << numContexts + m_presentationContexts.size()
                    << "). Some contexts will not be negotiated");
    }
    else
    {
        DCMNET_TRACE("Configured " << numContexts << " presentation contexts from config file");
        if (m_presentationContexts.size() > 0)
        {
            DCMNET_TRACE("Adding another " << m_presentationContexts.size() << " presentation contexts configured for SCU");
        }
    }

    OFListIterator(DicomPresentationContext) contIt = m_presentationContexts.begin();
    OFListConstIterator(DicomPresentationContext) endOfContList = m_presentationContexts.end();
    while ((contIt != endOfContList) && (nextFreePresID <= 255))
    {
        const Uint16 numTransferSyntaxes = OFstatic_cast(Uint16, (*contIt).transferSyntaxes.size());
        const char** transferSyntaxes = new const char*[numTransferSyntaxes];

        OFListIterator(OFString) syntaxIt = (*contIt).transferSyntaxes.begin();
        OFListIterator(OFString) endOfSyntaxList = (*contIt).transferSyntaxes.end();

        Uint16 sNum = 0;
        while (syntaxIt != endOfSyntaxList)
        {
            transferSyntaxes[sNum] = (*syntaxIt).c_str();
            ++syntaxIt;
            ++sNum;
        }

        condition = ASC_addPresentationContext(m_pParameters, OFstatic_cast(Uint8, nextFreePresID),
            (*contIt).abstractSyntaxName.c_str(), transferSyntaxes,
                                               numTransferSyntaxes, (*contIt).roleSelect);

        delete[] transferSyntaxes;
        transferSyntaxes = NULL;
        if (condition.good())
        {
            contIt++;
            nextFreePresID += 2;
        }
        else
        {
            DCMNET_ERROR("SetTransportLayerType Failed: " << DimseCondition::dump(tempStr, condition));
            return condition;
        }
    }

    numContexts = ASC_countPresentationContexts(m_pParameters);
    if (numContexts == 0)
    {
        DCMNET_ERROR("Cannot initialize network: No presentation contexts defined");
    }
    DCMNET_DEBUG("Configured a total of " << numContexts << " presentation contexts for SCU");
    return condition;
}

OFCondition DicomSCUBase::DropNetwork()
{
    OFCondition condition;

    if ((m_pAssociation != NULL) || (m_pNetwork != NULL) || (m_pParameters != NULL))
    {
        DCMNET_DEBUG("Cleaning up internal association and network structures");
    }
    if (m_pParameters)
    {
        ASC_destroyAssociationParameters(&m_pParameters); // 内部返回 EC_Normal 并将 parameters 置空
        m_pParameters = NULL;
        if (m_pAssociation)
        {
            m_pAssociation->params = NULL;
        }
    }
    condition = ASC_destroyAssociation(&m_pAssociation);
    if (condition.bad()) return condition;
    condition = ASC_dropNetwork(&m_pNetwork);

    return condition;
}

OFCondition DicomSCUBase::UseSecureConnection()
{
    OFString errorString;
    DcmTransportLayerStatus tlsStatus = TCS_ok;
    DcmTLSTransportLayer *pTlsTransport = new DcmTLSTransportLayer(DICOM_APPLICATION_REQUESTOR, NULL, false); // 不需要自己管理 DcmTLSTransportLayer 资源释放

    pTlsTransport->setPrivateKeyPasswd(DicomEnv::GetPrivateKeyPasswd().c_str());
    tlsStatus = pTlsTransport->setPrivateKeyFile(DicomEnv::GetPrivateKeyFile().c_str(), SSL_FILETYPE_PEM);
    if (tlsStatus != TCS_ok)
    {
        DCMNET_ERROR("DicomSCUBase::UseSecureConnection() set private key failed!");
        return makeDcmnetCondition(DULC_TLSERROR, OF_error, "set private key failed.");
    }
    tlsStatus = pTlsTransport->setCertificateFile(DicomEnv::GetCertificateFile().c_str(), SSL_FILETYPE_PEM);
    if (tlsStatus != TCS_ok)
    {
        DCMNET_ERROR("DicomSCUBase::UseSecureConnection() set certificate failed!");
        return makeDcmnetCondition(DULC_TLSERROR, OF_error, "set certificate failed.");
    }
    if (!pTlsTransport->checkPrivateKeyMatchesCertificate())
    {
        DCMNET_ERROR("DicomSCUBase::UseSecureConnection() check private key match certificate failed!");
        return makeDcmnetCondition(DULC_TLSERROR, OF_error, "check private key match certificate failed.");
    }

    OFList<OFString> trustedCerts = DicomEnv::GetTrustedCertificateList();
    OFListIterator(OFString) beginIt = trustedCerts.begin();
    OFListIterator(OFString) endIt = trustedCerts.end();
    while (beginIt != endIt)
    {
        tlsStatus = pTlsTransport->addTrustedCertificateFile(beginIt->c_str(), SSL_FILETYPE_PEM);
        if (tlsStatus != TCS_ok)
        {
            DCMNET_WARN("Add trusted certificate failed: " << *beginIt);
        }
        beginIt++;
    }
    pTlsTransport->setCertificateVerification(DCV_requireCertificate);

    OFCondition condition = ASC_setTransportLayer(m_pNetwork, pTlsTransport, 1); // 1 表示保持依赖关系，不需要自己管理 DcmTLSTransportLayer 资源释放
    if (condition.good())
    {
        condition = ASC_setTransportLayerType(m_pParameters, OFTrue);
        if (condition.bad())
        {
            DCMNET_ERROR("SetTransportLayerType Failed: " << DimseCondition::dump(errorString, condition));
        }
    }
    else
    {
        DCMNET_ERROR("SetTransportLayer Failed: " << DimseCondition::dump(errorString, condition));
    }
    return condition;
}

OFCondition DicomSCUBase::NegotiateAssociation()
{
    if (this->IsConnected()) return NET_EC_AlreadyConnected;

    OFString tempStr;
    DCMNET_INFO("Request Parameters:" << OFendl << ASC_dumpParameters(tempStr, m_pParameters, ASC_ASSOC_RQ));
    DCMNET_INFO("Requesting Association");
    OFCondition condition = ASC_requestAssociation(m_pNetwork, m_pParameters, &m_pAssociation);
    if (condition.bad())
    {
        if (condition == DUL_ASSOCIATIONREJECTED)
        {
            T_ASC_RejectParameters rej;
            ASC_getRejectParameters(m_pParameters, &rej);
            DCMNET_ERROR("Association Rejected:" << OFendl << ASC_printRejectParameters(tempStr, &rej));
            return condition;
        }
        else
        {
            DCMNET_ERROR("Association Request Failed: " << DimseCondition::dump(tempStr, condition));
            return condition;
        }
    }

    DCMNET_INFO("Association Parameters Negotiated:" << OFendl << ASC_dumpParameters(tempStr, m_pParameters, ASC_ASSOC_AC));
    if (ASC_countAcceptedPresentationContexts(m_pParameters) == 0)
    {
        DCMNET_ERROR("No Acceptable Presentation Contexts");
        return NET_EC_NoAcceptablePresentationContexts;
    }
    DCMNET_INFO("Association Accepted (Max Send PDV: " << OFstatic_cast(unsigned long, m_pAssociation->sendPDVLength) << ")");

    return EC_Normal;
}

OFCondition DicomSCUBase::ReleaseAssociation()
{
    OFString errorString;
    OFCondition condition;
    DCMNET_INFO("Releasing Association");
    condition = ASC_releaseAssociation(m_pAssociation);
    if (condition.bad())
    {
        DCMNET_ERROR("Association Release Failed: " << DimseCondition::dump(errorString, condition));
    }
    return condition;
}

OFCondition DicomSCUBase::AbortAssociation()
{
    OFString errorString;
    OFCondition condition;

    DCMNET_INFO("Aborting Association");
    condition = ASC_abortAssociation(m_pAssociation);
    if (condition.bad())
    {
        DCMNET_ERROR("Association Abort Failed: " << DimseCondition::dump(errorString, condition));
    }
    return condition;
}

OFBool DicomSCUBase::IsConnected()
{
    OFBool retValue = OFFalse;
    if (m_pAssociation != NULL && m_pAssociation->DULassociation != NULL)
    {
        retValue = OFTrue;
    }
    return retValue;
}

OFCondition DicomSCUBase::EchoUser()
{
    DIC_US msgId = m_pAssociation->nextMsgID++;
    DIC_US status;
    DcmDataset *statusDetail = NULL;

    DCMNET_INFO("Sending Echo Request (MsgID " << msgId << ")");
    OFCondition condition = DIMSE_echoUser(m_pAssociation, msgId, m_blockMode, m_dimseTimeoutSeconds, &status, &statusDetail);
    if (condition.good())
    {
        DCMNET_INFO("Received Echo Response (" << DU_cechoStatusString(status) << ")");
    }
    else
    {
        OFString temp_str;
        DCMNET_ERROR("Echo Failed: " << DimseCondition::dump(temp_str, condition));
    }

    if (statusDetail != NULL)
    {
        DCMNET_DEBUG("Status Detail (should never be any):" << OFendl << DcmObject::PrintHelper(*statusDetail));
        delete statusDetail;
    }

    return condition;
}

OFCondition DicomSCUBase::FindUser(const char *abstractSyntax, OFList<OFString> *pOverrideKeys,
                                   T_DIMSE_C_FindRQ *pRequest, T_DIMSE_C_FindRSP *pRsponse,
                                   DIMSE_FindUserCallback callback, void *callbackData)
{
    T_ASC_PresentationContextID presId;
    DcmFileFormat dcmFileFormat;
    OFString tempString;
    OFCondition condition;

    OFListConstIterator(OFString) keyPath = pOverrideKeys->begin();
    OFListConstIterator(OFString) lastKey = pOverrideKeys->end();
    DcmDataset* requestIdentifiers = dcmFileFormat.getDataset();
    DcmPathProcessor dcmPathProcessor;
    dcmPathProcessor.setItemWildcardSupport(OFFalse);
    dcmPathProcessor.checkPrivateReservations(OFFalse);
    while (keyPath != lastKey)
    {
        condition = dcmPathProcessor.applyPathWithValue(requestIdentifiers, *keyPath);
        if (condition.bad())
        {
            DCMNET_ERROR("Bad override key/path: " << *keyPath << ": " << condition.text());
            return condition;
        }
        keyPath++;
    }

    presId = ASC_findAcceptedPresentationContextID(m_pAssociation, abstractSyntax);
    if (presId == 0)
    {
        DCMNET_ERROR("No presentation context");
        return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
    }

    bzero(OFreinterpret_cast(char*, pRequest), sizeof(T_DIMSE_C_FindRQ));
    strcpy(pRequest->AffectedSOPClassUID, abstractSyntax);
    pRequest->DataSetType = DIMSE_DATASET_PRESENT;
    pRequest->Priority = DIMSE_PRIORITY_MEDIUM;
    pRequest->MessageID = m_pAssociation->nextMsgID++;
    DCMNET_INFO("Sending Find Request (MsgID " << pRequest->MessageID << ")");
    DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRequest, DIMSE_OUTGOING, NULL, presId));
    DCMNET_INFO("Request Identifiers:" << OFendl << DcmObject::PrintHelper(*requestIdentifiers));

    DcmDataset *statusDetail = NULL;
    // TODO 回调处理
    condition = DIMSE_findUser(m_pAssociation, presId, pRequest, requestIdentifiers, callback, callbackData,
                               m_blockMode, m_dimseTimeoutSeconds,
                               pRsponse, &statusDetail);
    if (condition.good())
    {
        DCMNET_INFO("Received Final Find Response (" << DU_cfindStatusString(pRsponse->DimseStatus) << ")");
        DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRsponse, DIMSE_INCOMING));
    }
    else
    {
        DCMNET_ERROR("Find Failed, query keys:");
        DCMNET_ERROR(DcmObject::PrintHelper(dcmFileFormat));
        DCMNET_ERROR(DimseCondition::dump(tempString, condition));
    }

    if (statusDetail != NULL)
    {
        DCMNET_DEBUG("Status Detail:" << OFendl << DcmObject::PrintHelper(*statusDetail));
        delete statusDetail;
    }

    return condition;
}

OFCondition DicomSCUBase::GetUser(const char *abstractSyntax, OFList<OFString> *pOverrideKeys,
                                  T_DIMSE_C_GetRQ *pRequest, T_DIMSE_C_GetRSP *pRsponse,
                                  DIMSE_GetUserCallbackEx callback, void *callbackData,
                                  DIMSE_SubOpProviderCallbackEx subOpCallback, void *subOpCallbackData)
{
    T_ASC_PresentationContextID presentationId;
    DcmFileFormat               dcmFileFormat;
    OFString                    tempString;
    OFCondition                 condition;

    DcmDataset* requestIdentifiers = dcmFileFormat.getDataset();
    DcmPathProcessor dcmPathProcessor;
    dcmPathProcessor.setItemWildcardSupport(OFFalse);
    dcmPathProcessor.checkPrivateReservations(OFFalse);

    OFListConstIterator(OFString) keyPath = pOverrideKeys->begin();
    OFListConstIterator(OFString) lastKey = pOverrideKeys->end();
    while (keyPath != lastKey)
    {
        condition = dcmPathProcessor.applyPathWithValue(requestIdentifiers, *keyPath);
        if (condition.bad())
        {
            DCMNET_ERROR("Bad override key/path: " << *keyPath << ": " << condition.text());
            return condition;
        }
        keyPath++;
    }

    presentationId = ASC_findAcceptedPresentationContextID(m_pAssociation, abstractSyntax);
    if (presentationId == 0)
    {
        DCMNET_ERROR("No presentation context");
        return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
    }

    bzero(OFreinterpret_cast(char*, pRequest), sizeof(T_DIMSE_C_GetRQ));
    strcpy(pRequest->AffectedSOPClassUID, abstractSyntax);
    pRequest->DataSetType = DIMSE_DATASET_PRESENT;
    pRequest->Priority = DIMSE_PRIORITY_MEDIUM;
    pRequest->MessageID = m_pAssociation->nextMsgID++;
    DCMNET_INFO("Sending C-GET Request (MsgID " << pRequest->MessageID << ")");
    DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRequest, DIMSE_OUTGOING, NULL, presentationId));
    DCMNET_INFO("Request Identifiers:" << OFendl << DcmObject::PrintHelper(*requestIdentifiers));

    DcmDataset *statusDetail = NULL;
    DcmDataset *responseIdentifiers = NULL;
    if (condition.good())
    {
        condition = DIMSE_getUserEx(m_pAssociation, presentationId, pRequest, requestIdentifiers, callback, callbackData,
                                    m_blockMode, m_dimseTimeoutSeconds,
                                    m_pNetwork, subOpCallback, subOpCallbackData,
                                    pRsponse, &statusDetail, &responseIdentifiers);
    }
    if (condition.good())
    {
        DCMNET_INFO("Received Final Get Response (" << DU_cfindStatusString(pRsponse->DimseStatus) << ")");
        DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRsponse, DIMSE_INCOMING));
    }
    else
    {
        DCMNET_ERROR("Get Failed, query keys:");
        DCMNET_ERROR(DcmObject::PrintHelper(dcmFileFormat));
        DCMNET_ERROR(DimseCondition::dump(tempString, condition));
    }

    if (statusDetail != NULL)
    {
        DCMNET_DEBUG("Status Detail:" << OFendl << DcmObject::PrintHelper(*statusDetail));
        delete statusDetail;
    }
    if (responseIdentifiers != NULL)
    {
        DCMNET_DEBUG("Status Detail:" << OFendl << DcmObject::PrintHelper(*responseIdentifiers));
        delete responseIdentifiers;
    }

    return condition;
}

OFCondition DicomSCUBase::MoveUser(const char *abstractSyntax, OFList<OFString> *pOverrideKeys,
                                   T_DIMSE_C_MoveRQ *pRequest, T_DIMSE_C_MoveRSP *pResponse,
                                   DIMSE_MoveUserCallback callback, void *callbackData,
                                   DIMSE_SubOpProviderCallback subOpCallback, void *subOpCallbackData)
{
    T_ASC_PresentationContextID  presentationID = 0;
    DcmFileFormat                dcmFileFormat;
    DcmDataset                  *pResponseIdentifiers = NULL;
    DcmDataset                  *pStatusDetail = NULL;
    OFString                     tempString;
    OFCondition                  condition = EC_Normal;

    DcmDataset* requestIdentifiers = dcmFileFormat.getDataset();
    DcmPathProcessor dcmPathProcessor;
    dcmPathProcessor.setItemWildcardSupport(OFFalse);
    dcmPathProcessor.checkPrivateReservations(OFFalse);

    OFListConstIterator(OFString) keyPath = pOverrideKeys->begin();
    OFListConstIterator(OFString) lastKey = pOverrideKeys->end();
    while (keyPath != lastKey)
    {
        condition = dcmPathProcessor.applyPathWithValue(requestIdentifiers, *keyPath);
        if (condition.bad())
        {
            DCMNET_ERROR("Bad override key/path: " << *keyPath << ": " << condition.text());
            return condition;
        }
        keyPath++;
    }

    presentationID = ASC_findAcceptedPresentationContextID(m_pAssociation, abstractSyntax);
    if (presentationID == 0)
    {
        return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
    }

    bzero(OFreinterpret_cast(char*, pRequest), sizeof(T_DIMSE_C_MoveRQ));
    pRequest->MessageID = m_pAssociation->nextMsgID++;
    strcpy(pRequest->AffectedSOPClassUID, abstractSyntax);
    strcpy(pRequest->MoveDestination, m_destAETitle.c_str());
    pRequest->Priority = DIMSE_PRIORITY_MEDIUM;
    pRequest->DataSetType = DIMSE_DATASET_PRESENT;
    DCMNET_DEBUG("Sending Move Request (MsgID " << pRequest->MessageID << ")");
    DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRequest, DIMSE_OUTGOING, NULL, presentationID));

    if (condition.good())
    {
        condition = DIMSE_moveUser(m_pAssociation, presentationID, pRequest, requestIdentifiers,
                                   callback, callbackData, m_blockMode, m_dimseTimeoutSeconds, m_pNetwork,
                                   subOpCallback, subOpCallbackData,
                                   pResponse, &pStatusDetail, &pResponseIdentifiers);
    }

    if (condition.good())
    {
        DCMNET_INFO("Received Final Move Response (" << DU_cmoveStatusString(pResponse->DimseStatus) << ")");
        DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pResponse, DIMSE_INCOMING));
        if (pResponseIdentifiers != NULL)
        {
            DCMNET_DEBUG("Response Identifiers:" << OFendl << DcmObject::PrintHelper(*pResponseIdentifiers));
        }
    }
    else
    {
        DCMNET_ERROR("Move Request Failed: " << DimseCondition::dump(tempString, condition));
    }
    if (pStatusDetail != NULL)
    {
        DCMNET_DEBUG("Status Detail:" << OFendl << DcmObject::PrintHelper(*pStatusDetail));
        delete pStatusDetail;
    }

    if (pResponseIdentifiers != NULL)
    {
        delete pResponseIdentifiers;
    }

    return condition;
}

OFCondition DicomSCUBase::StoreUser(DcmDataset *pStoreDataset,
                                    T_DIMSE_C_StoreRQ *pRequest, T_DIMSE_C_StoreRSP *pResponse,
                                    DIMSE_StoreUserCallback callback, void *callbackData)
{
    OFString tempString;
    OFCondition condition = EC_Normal;
    DIC_UI sopClass;
    DIC_UI sopInstance;
    DcmDataset *pStatusDetail = NULL;
    T_ASC_PresentationContextID presentationID;

    if (!DU_findSOPClassAndInstanceInDataSet(pStoreDataset, sopClass, sopInstance, OFFalse))
    {
        DCMNET_ERROR("No SOP Class or Instance UID in store dataset");
        condition = DIMSE_BADDATA;
    }

    DcmXfer datasetXfer(pStoreDataset->getOriginalXfer());
    if (datasetXfer.getXfer() != EXS_Unknown)
    {
        presentationID = ASC_findAcceptedPresentationContextID(m_pAssociation, sopClass, datasetXfer.getXferID());
    }
    else
    {
        presentationID = ASC_findAcceptedPresentationContextID(m_pAssociation, sopClass);
    }
    if (presentationID == 0)
    {
        DCMNET_ERROR("No presentation context");
        return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
    }

    bzero(OFreinterpret_cast(char*, pRequest), sizeof(T_DIMSE_C_StoreRQ));
    strcpy(pRequest->AffectedSOPClassUID, sopClass);
    strcpy(pRequest->AffectedSOPInstanceUID, sopInstance);
    pRequest->DataSetType = DIMSE_DATASET_PRESENT;
    pRequest->Priority = DIMSE_PRIORITY_MEDIUM;
    pRequest->MessageID = m_pAssociation->nextMsgID++;
    DCMNET_INFO("Sending C-STORE Request (MsgID " << pRequest->MessageID << ")");

    if (condition.good())
    {
        condition = DIMSE_storeUser(m_pAssociation, presentationID, pRequest, NULL, pStoreDataset,
                                    callback, callbackData,
                                    m_blockMode, m_dimseTimeoutSeconds, pResponse, &pStatusDetail);
    }

    if (condition == EC_Normal)
    {
        DCMNET_INFO("Received Store Response (" << DU_cstoreStatusString(pResponse->DimseStatus) << ")");
        DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pResponse, DIMSE_INCOMING, NULL, presentationID));
    }
    else
    {
        DCMNET_ERROR("Store Failed:" << OFendl << DimseCondition::dump(tempString, condition));
    }

    if (pStatusDetail != NULL)
    {
        DCMNET_DEBUG("Status Detail:" << OFendl << DcmObject::PrintHelper(*pStatusDetail));
        delete pStatusDetail;
    }

    return condition;
}

OFCondition DicomSCUBase::DIMSE_getUserEx(T_ASC_Association *pAssociation, T_ASC_PresentationContextID presentationID, T_DIMSE_C_GetRQ *pRequest,
                                          DcmDataset *pRequestIdentifiers, DIMSE_GetUserCallbackEx callback, void *callbackData,
                                          T_DIMSE_BlockingMode blockMode, int timeout, T_ASC_Network *pNetwork,
                                          DIMSE_SubOpProviderCallbackEx subOpCallback, void *pSubOpCallbackData,
                                          T_DIMSE_C_GetRSP *pResponse, DcmDataset **ppStatusDetail, DcmDataset **ppResponseIdentifers)
{
    OFCondition condition;
    OFString tempString;

    int responseCount = 0;
    DIC_US msgId;
    OFBool continueSession = OFTrue;
    T_DIMSE_Message requestMSG, responseMSG;

    if (pRequestIdentifiers == NULL) return DIMSE_NULLKEY;

    bzero((char*)&requestMSG, sizeof(requestMSG));
    bzero((char*)&responseMSG, sizeof(responseMSG));

    requestMSG.CommandField = DIMSE_C_GET_RQ;
    pRequest->DataSetType = DIMSE_DATASET_PRESENT;
    requestMSG.msg.CGetRQ = *pRequest;

    msgId = pRequest->MessageID;

    condition = DIMSE_sendMessageUsingMemoryData(pAssociation, presentationID, &requestMSG,NULL, pRequestIdentifiers, NULL, NULL);
    if (condition.bad())
    {
        DCMNET_ERROR("Failed sending C-GET request: " << DimseCondition::dump(tempString, condition));
        return condition;
    }

    while (continueSession)
    {
        T_ASC_PresentationContextID recvPresentationID = 0;

        condition = DIMSE_receiveCommand(pAssociation, blockMode, timeout, &recvPresentationID, &responseMSG, ppStatusDetail);
        if (condition.bad())
        {
            DCMNET_ERROR("Failed receiving C-GET receiving DIMSE command" << DimseCondition::dump(tempString, condition));
            return condition;
        }

        if (responseMSG.CommandField == DIMSE_C_GET_RSP) // C-GET 响应处理
        {
            ++responseCount;
            *pResponse = responseMSG.msg.CGetRSP;
            DCMNET_INFO("Received C-GET Response");
            DCMNET_DEBUG(DIMSE_dumpMessage(tempString, responseMSG, DIMSE_INCOMING, NULL, recvPresentationID));
            if (callback != NULL)
            {
                callback(callbackData, pRequest, responseCount, pResponse, continueSession);
            }
        }
        else if (responseMSG.CommandField == DIMSE_C_STORE_RQ) // C-STORE 存储请求
        {
            T_DIMSE_C_StoreRQ cstoreRequest = responseMSG.msg.CStoreRQ;

            DCMNET_INFO("Received C-STORE Request");
            DCMNET_DEBUG(DIMSE_dumpMessage(tempString, responseMSG, DIMSE_INCOMING, NULL, recvPresentationID));

            Uint16 cstoreReturnStatus = 0;
            DcmDataset *pResponseDataSet = NULL;
            condition = DIMSE_receiveDataSetInMemory(m_pAssociation, m_blockMode, m_dimseTimeoutSeconds, &recvPresentationID, &pResponseDataSet, NULL, NULL);
            if (condition.bad())
            {
                DCMNET_ERROR("Failed receiving C-GET receiving DIMSE command" << DimseCondition::dump(tempString, condition));
                cstoreReturnStatus = STATUS_STORE_Error_CannotUnderstand;
                continueSession = OFFalse;
            }
            else
            {
                if (subOpCallback)
                {
                    subOpCallback(pSubOpCallbackData, &cstoreRequest, &recvPresentationID, continueSession, cstoreReturnStatus, pResponseDataSet);
                }
            }

            T_DIMSE_Message cstoreResponseMSG;
            bzero((char*)&cstoreResponseMSG, sizeof(cstoreResponseMSG));
            cstoreResponseMSG.CommandField = DIMSE_C_STORE_RSP;
            T_DIMSE_C_StoreRSP &storeRsp = cstoreResponseMSG.msg.CStoreRSP;
            storeRsp.MessageIDBeingRespondedTo = cstoreRequest.MessageID;
            storeRsp.DimseStatus = cstoreReturnStatus;
            storeRsp.DataSetType = DIMSE_DATASET_NULL;

            OFStandard::strlcpy(storeRsp.AffectedSOPClassUID, cstoreRequest.AffectedSOPClassUID, sizeof(storeRsp.AffectedSOPClassUID));
            OFStandard::strlcpy(storeRsp.AffectedSOPInstanceUID, cstoreRequest.AffectedSOPInstanceUID, sizeof(storeRsp.AffectedSOPInstanceUID));
            storeRsp.opts = O_STORE_AFFECTEDSOPCLASSUID | O_STORE_AFFECTEDSOPINSTANCEUID;

            DCMNET_INFO("Sending C-STORE Response");
            DCMNET_DEBUG(DIMSE_dumpMessage(tempString, cstoreResponseMSG, DIMSE_OUTGOING, NULL, presentationID));

            condition = DIMSE_sendMessageUsingMemoryData(m_pAssociation, presentationID, &cstoreResponseMSG, NULL, NULL, NULL, NULL);
            if (condition.bad())
            {
                DCMNET_ERROR("Failed sending C-STORE response: " << DimseCondition::dump(tempString, condition));
            }
        }
        else
        {
            DCMNET_ERROR("Expected C-GET response or C-STORE request but received DIMSE command 0x"
                         << std::hex << std::setfill('0') << std::setw(4)
                         << OFstatic_cast(unsigned int, responseMSG.CommandField));
            DCMNET_DEBUG(DIMSE_dumpMessage(tempString, responseMSG, DIMSE_INCOMING, NULL, presentationID));
            condition = DIMSE_BADCOMMANDTYPE;
            continueSession = OFFalse;
        }
    }

    return condition;
}
