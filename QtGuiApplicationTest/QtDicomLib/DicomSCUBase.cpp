#include "DicomSCUBase.h"

#include "IDicomSCUObserver.h"

// DCMTK Headers
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcpath.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmnet/cond.h>

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
    , m_maxReceivePDU(16384)
    , m_peerHostName("localhost")
    , m_peerAETitle("MGIUSDICOM")
    , m_appAETitle("MGIUSDOCTOR")
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

OFCondition DicomSCUBase::UseSecureConnection(DcmTransportLayer *transLayer)
{
    OFString errorString;
    OFCondition condition = ASC_setTransportLayer(m_pNetwork, transLayer, OFFalse);
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
                                   DIMSE_GetUserCallback callback, void *callbackData,
                                   DIMSE_SubOpProviderCallback subOpCallback, void *subOpCallbackData)
{
    T_ASC_PresentationContextID presId;
    DcmFileFormat dcmFileFormat;
    OFString tempString;
    OFCondition condition;

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

    presId = ASC_findAcceptedPresentationContextID(m_pAssociation, abstractSyntax);
    if (presId == 0)
    {
        DCMNET_ERROR("No presentation context");
        return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
    }

    strcpy(pRequest->AffectedSOPClassUID, abstractSyntax);
    pRequest->DataSetType = DIMSE_DATASET_PRESENT;
    pRequest->Priority = DIMSE_PRIORITY_MEDIUM;
    pRequest->MessageID = m_pAssociation->nextMsgID++;
    DCMNET_INFO("Sending Find Request (MsgID " << pRequest->MessageID << ")");
    DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRequest, DIMSE_OUTGOING, NULL, presId));
    DCMNET_INFO("Request Identifiers:" << OFendl << DcmObject::PrintHelper(*requestIdentifiers));

    DcmDataset *statusDetail = NULL;
    DcmDataset *responseIdentifiers = NULL;
    // TODO 回调处理
    condition = DIMSE_getUser(m_pAssociation, presId, pRequest, requestIdentifiers, callback, callbackData,
                              m_blockMode, m_dimseTimeoutSeconds,
                              m_pNetwork, subOpCallback, subOpCallbackData,
                              pRsponse, &statusDetail, &responseIdentifiers);
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
    if (responseIdentifiers != NULL)
    {
        DCMNET_DEBUG("Status Detail:" << OFendl << DcmObject::PrintHelper(*responseIdentifiers));
        delete responseIdentifiers;
    }

    return condition;
}

OFCondition DicomSCUBase::MoveUser()
{
    OFCondition condition;
    return condition;
}

OFCondition DicomSCUBase::StoreUser()
{
    OFCondition condition;
    return condition;
}
