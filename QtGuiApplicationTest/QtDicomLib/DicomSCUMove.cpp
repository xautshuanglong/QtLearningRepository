#include "DicomSCUMove.h"

// DCMTK Headers
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmnet/diutil.h>

static const char* gsMoveModelUID[] =
{
    UID_MOVEPatientRootQueryRetrieveInformationModel,
    UID_MOVEStudyRootQueryRetrieveInformationModel,
    UID_RETIRED_MOVEPatientStudyOnlyQueryRetrieveInformationModel,
};

/* DICOM standard transfer syntaxes */
static const char* transferSyntaxes[] =
{
    UID_LittleEndianImplicitTransferSyntax, /* default xfer syntax first */
    UID_LittleEndianExplicitTransferSyntax,
    UID_BigEndianExplicitTransferSyntax,
    UID_JPEGProcess1TransferSyntax,
    UID_JPEGProcess2_4TransferSyntax,
    UID_JPEGProcess3_5TransferSyntax,
    UID_JPEGProcess6_8TransferSyntax,
    UID_JPEGProcess7_9TransferSyntax,
    UID_JPEGProcess10_12TransferSyntax,
    UID_JPEGProcess11_13TransferSyntax,
    UID_JPEGProcess14TransferSyntax,
    UID_JPEGProcess15TransferSyntax,
    UID_JPEGProcess16_18TransferSyntax,
    UID_JPEGProcess17_19TransferSyntax,
    UID_JPEGProcess20_22TransferSyntax,
    UID_JPEGProcess21_23TransferSyntax,
    UID_JPEGProcess24_26TransferSyntax,
    UID_JPEGProcess25_27TransferSyntax,
    UID_JPEGProcess28TransferSyntax,
    UID_JPEGProcess29TransferSyntax,
    UID_JPEGProcess14SV1TransferSyntax,
    UID_RLELosslessTransferSyntax,
    UID_DeflatedExplicitVRLittleEndianTransferSyntax,
    UID_JPEGLSLosslessTransferSyntax,
    UID_JPEGLSLossyTransferSyntax,
    UID_JPEG2000LosslessOnlyTransferSyntax,
    UID_JPEG2000TransferSyntax,
    UID_JPEG2000Part2MulticomponentImageCompressionLosslessOnlyTransferSyntax,
    UID_JPEG2000Part2MulticomponentImageCompressionTransferSyntax,
    UID_MPEG2MainProfileAtMainLevelTransferSyntax,
    UID_MPEG2MainProfileAtHighLevelTransferSyntax,
    UID_MPEG4HighProfileLevel4_1TransferSyntax,
    UID_MPEG4BDcompatibleHighProfileLevel4_1TransferSyntax,
    UID_MPEG4HighProfileLevel4_2_For2DVideoTransferSyntax,
    UID_MPEG4HighProfileLevel4_2_For3DVideoTransferSyntax,
    UID_MPEG4StereoHighProfileLevel4_2TransferSyntax,
    UID_HEVCMainProfileLevel5_1TransferSyntax,
    UID_HEVCMain10ProfileLevel5_1TransferSyntax,
};

struct StoreCallbackData
{
    char              *pImageFilename;
    DcmFileFormat     *pDcmFileFormat;
    T_ASC_Association *pAssociation;
};

DicomSCUMove::DicomSCUMove()
    : DicomSCUBase()
{
    bzero(OFreinterpret_cast(char*, &m_request), sizeof(m_request));
}

DicomSCUMove::~DicomSCUMove()
{
}

OFCondition DicomSCUMove::ExcuteOperation()
{
    return EC_NotYetImplemented;
}

OFCondition DicomSCUMove::PerformMove(EnumMoveModel moveModel)
{
    OFCondition condition;

    condition = this->InitNetwork();
    if (condition.good())
    {
        this->UseSecureConnection();
    }
    if (condition.good())
    {
        condition = this->NegotiateAssociation();
    }
    if (condition.good())
    {
        condition = this->MoveUser(gsMoveModelUID[moveModel], &m_overrideKeys,
                                  &m_request, &m_response,
                                  DicomSCUMove::MoveUserCallback, this,
                                  DicomSCUMove::SubOpProviderCallback, this);
        if (condition.good())
        {
            condition = this->ReleaseAssociation();
        }
        else if (condition == DUL_PEERREQUESTEDRELEASE)
        {
            condition = this->AbortAssociation();
        }
        else
        {
            OFString errorString;
            DCMNET_ERROR("Failed peforming this->GetUser --> %s" << DimseCondition::dump(errorString, condition));
        }
    }

    condition = this->DropNetwork();
    if (condition.bad())
    {
        OFString errorString;
        DCMNET_ERROR("Failed dropping network:" << DimseCondition::dump(errorString, condition));
    }

    return condition;
}

void DicomSCUMove::HandleMoveCallback(T_DIMSE_C_MoveRQ *pRequest, int responseCount, T_DIMSE_C_MoveRSP *pResponse)
{
    OFCondition condition = EC_Normal;
    OFString tempString;
    DCMNET_INFO("Received Move Response " << responseCount
                << " (" << DU_cmoveStatusString(pResponse->DimseStatus) << ")");
    DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pResponse, DIMSE_INCOMING));
}

void DicomSCUMove::MoveUserCallback(void *pCallbackData, T_DIMSE_C_MoveRQ *pRequest, int responseCount, T_DIMSE_C_MoveRSP *pResponse)
{
    DicomSCUMove *pCallback = OFreinterpret_cast(DicomSCUMove*, pCallbackData);
    if (pCallback)
    {
        pCallback->HandleMoveCallback(pRequest, responseCount, pResponse);
    }
}

void DicomSCUMove::HandleSubOperationCallback(T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc)
{
    OFCondition condition = EC_Normal;
    OFString tempString;

    if (pNetwork != NULL)
    {
        if (*ppSubOpAssoc == NULL)
        {
            this->AcceptSubAssociation(pNetwork, ppSubOpAssoc);
        }
        else
        {
            this->SubOperation_SCP(ppSubOpAssoc);
        }
    }
}

void DicomSCUMove::SubOpProviderCallback(void *pSubOpCallbackData, T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc)
{
    DicomSCUMove *pCallback = OFreinterpret_cast(DicomSCUMove*, pSubOpCallbackData);
    if (pCallback)
    {
        pCallback->HandleSubOperationCallback(pNetwork, ppSubOpAssoc);
    }
}

void DicomSCUMove::AcceptSubAssociation(T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc)
{
    OFCondition condition = EC_Normal;
    OFString tempString;
    const char *knownAbstractSyntaxes[] =
    {
        UID_VerificationSOPClass
    };

    condition = ASC_receiveAssociation(pNetwork, ppSubOpAssoc, m_maxReceivePDU);
    if (condition.good())
    {
        DCMNET_INFO("Sub-Association Received");
        DCMNET_DEBUG("Parameters:" << OFendl << ASC_dumpParameters(tempString, (*ppSubOpAssoc)->params, ASC_ASSOC_RQ));
        condition = ASC_acceptContextsWithPreferredTransferSyntaxes((*ppSubOpAssoc)->params,
                                                                    knownAbstractSyntaxes,
                                                                    DIM_OF(knownAbstractSyntaxes),
                                                                    transferSyntaxes,
                                                                    DIM_OF(transferSyntaxes));
        if (condition.good())
        {
            condition = ASC_acceptContextsWithPreferredTransferSyntaxes((*ppSubOpAssoc)->params,
                                                                        dcmAllStorageSOPClassUIDs,
                                                                        numberOfDcmLongSCUStorageSOPClassUIDs,
                                                                        transferSyntaxes,
                                                                        DIM_OF(transferSyntaxes));
        }
    }
    if (condition.good())
    {
        condition = ASC_acknowledgeAssociation(*ppSubOpAssoc);
    }
    if (condition.good())
    {
        DCMNET_INFO("Sub-Association Acknowledged (Max Send PDV: " << (*ppSubOpAssoc)->sendPDVLength << ")");
        if (ASC_countAcceptedPresentationContexts((*ppSubOpAssoc)->params) == 0)
        {
            DCMNET_INFO("    (but no valid presentation contexts)");
        }
        DCMNET_DEBUG(ASC_dumpParameters(tempString, (*ppSubOpAssoc)->params, ASC_ASSOC_AC));
    }
    else
    {
        DCMNET_ERROR(DimseCondition::dump(tempString, condition));
        ASC_dropAssociation(*ppSubOpAssoc);
        ASC_destroyAssociation(ppSubOpAssoc);
    }
}

void DicomSCUMove::SubOperation_SCP(T_ASC_Association **ppSubOpAssoc)
{
    OFString tempString;
    T_DIMSE_Message recvMSG;
    T_ASC_PresentationContextID presentationID;

    if (ASC_dataWaiting(*ppSubOpAssoc, 0))
    {
        OFCondition condition = DIMSE_receiveCommand(*ppSubOpAssoc, m_blockMode, m_dimseTimeoutSeconds, &presentationID, &recvMSG, NULL);
        if (condition == EC_Normal)
        {
            switch (recvMSG.CommandField)
            {
            case DIMSE_C_ECHO_RQ: // process C-ECHO-Request
                condition = this->SubOperation_EchoSCP(*ppSubOpAssoc, &recvMSG, presentationID);
                break;
            case DIMSE_C_STORE_RQ: // process C-STORE-Request
                condition = this->SubOperation_StoreSCP(*ppSubOpAssoc, &recvMSG, presentationID);
                break;
            default:
                condition = DIMSE_BADCOMMANDTYPE;
                DCMNET_ERROR("Expected C-ECHO or C-STORE request but received DIMSE command 0x"
                            << std::hex << std::setfill('0') << std::setw(4) << OFstatic_cast(unsigned, recvMSG.CommandField));
                DCMNET_DEBUG(DIMSE_dumpMessage(tempString, recvMSG, DIMSE_INCOMING, NULL, presentationID));
                break;
            }
        }
        if (condition == DUL_PEERREQUESTEDRELEASE)
        {
            condition = ASC_acknowledgeRelease(*ppSubOpAssoc);
            ASC_dropSCPAssociation(*ppSubOpAssoc);
            ASC_destroyAssociation(ppSubOpAssoc);
            return;
        }
        else if (condition == DUL_PEERABORTEDASSOCIATION)
        {
        }
        else if (condition != EC_Normal)
        {
            DCMNET_ERROR("DIMSE failure (aborting sub-association): " << DimseCondition::dump(tempString, condition));
            condition = ASC_abortAssociation(*ppSubOpAssoc);
        }

        if (condition != EC_Normal)
        {
            ASC_dropAssociation(*ppSubOpAssoc);
            ASC_destroyAssociation(ppSubOpAssoc);
        }
    }
}

OFCondition DicomSCUMove::SubOperation_EchoSCP(T_ASC_Association *pSubOpAssociation, T_DIMSE_Message *pReceivedMsg, T_ASC_PresentationContextID presentationID)
{
    OFString tempString;
    T_DIMSE_C_EchoRQ *pRequest = &pReceivedMsg->msg.CEchoRQ;

    DCMNET_INFO("Received Echo Request (MsgID " << pRequest->MessageID << ")");
    DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRequest, DIMSE_INCOMING, NULL, presentationID));

    OFCondition condition = DIMSE_sendEchoResponse(pSubOpAssociation, presentationID, pRequest, STATUS_Success, NULL);
    if (condition.bad())
    {
        DCMNET_ERROR("Echo SCP Failed: " << DimseCondition::dump(tempString, condition));
    }
    return condition;
}

OFCondition DicomSCUMove::SubOperation_StoreSCP(T_ASC_Association *pSubOpAssociation, T_DIMSE_Message *pReceivedMsg, T_ASC_PresentationContextID presentationID)
{
    OFString tempString;
    OFCondition condition = EC_Normal;
    T_DIMSE_C_StoreRQ *pRequest = &pReceivedMsg->msg.CStoreRQ;
    
    char imageFileName[2048];
    sprintf(imageFileName, "%s.%s",
            dcmSOPClassUIDToModality(pRequest->AffectedSOPClassUID),
            pRequest->AffectedSOPInstanceUID);

    DCMNET_INFO("Received Store Request (MsgID " << pRequest->MessageID << ", "
                << dcmSOPClassUIDToModality(pRequest->AffectedSOPClassUID, "OT") << ")");
    DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRequest, DIMSE_INCOMING, NULL, presentationID));

    DcmFileFormat dcmFormatFile;
    StoreCallbackData callbackData;
    callbackData.pAssociation = pSubOpAssociation;
    callbackData.pImageFilename = imageFileName;
    callbackData.pDcmFileFormat = &dcmFormatFile;

    // store SourceApplicationEntityTitle in metaheader
    if (pSubOpAssociation && pSubOpAssociation ->params)
    {
        const char *aet = pSubOpAssociation ->params->DULparams.callingAPTitle;
        if (aet) dcmFormatFile.getMetaInfo()->putAndInsertString(DCM_SourceApplicationEntityTitle, aet);
    }

    DcmDataset *pDataset = dcmFormatFile.getDataset();
    condition = DIMSE_storeProvider(pSubOpAssociation, presentationID, pRequest, NULL, OFTrue,
                                    &pDataset, DicomSCUMove::StoreProviderCallback, OFreinterpret_cast(void*, &callbackData),
                                    m_blockMode, m_dimseTimeoutSeconds);

    if (condition.bad())
    {
        DCMNET_ERROR("Store SCP Failed: " << DimseCondition::dump(tempString, condition));
        if (strcmp(imageFileName, NULL_DEVICE_NAME) != 0)
        {
            OFStandard::deleteFile(imageFileName);
        }
    }

    return condition;
}

void DicomSCUMove::StoreProviderCallback(void *pCallbackData, T_DIMSE_StoreProgress *pProgress, T_DIMSE_C_StoreRQ *pRequest,
                                         char *pImageFilename, DcmDataset **ppImageDataSet, T_DIMSE_C_StoreRSP *pResponse,
                                         DcmDataset **ppStatusDetail)
{
    DIC_UI sopClass;
    DIC_UI sopInstance;

    switch (pProgress->state)
    {
    case DIMSE_StoreBegin:
        DCMNET_DEBUG("DIMSE_StoreBegin ========================");
        break;
    case DIMSE_StoreProgressing:
        DCMNET_DEBUG("DIMSE_StoreProgressing ========================");
        break;
    case DIMSE_StoreEnd:
        DCMNET_DEBUG("DIMSE_StoreEnd ========================");
        break;
    default:
        break;
    }

    if (pProgress->state == DIMSE_StoreEnd)
    {
        *ppStatusDetail = NULL;

        if ((ppImageDataSet != NULL) && (*ppImageDataSet != NULL))
        {
            StoreCallbackData *cbdata = OFstatic_cast(StoreCallbackData*, pCallbackData);
            OFString filename;
            OFStandard::combineDirAndFilename(filename, "./", pImageFilename, OFTrue);
            if (OFStandard::fileExists(filename))
            {
                DCMNET_WARN("DICOM file already exists, overwriting: " << filename);
            }

            E_TransferSyntax xfer = (*ppImageDataSet)->getOriginalXfer();
            OFCondition condition = cbdata->pDcmFileFormat->saveFile(filename.c_str(), xfer,
                                                                EET_ExplicitLength, EGL_recalcGL,
                                                                EPD_withoutPadding,
                                                                OFstatic_cast(Uint32, 0),
                                                                OFstatic_cast(Uint32, 0),
                                                                EWM_fileformat);
            if (condition.bad())
            {
                DCMNET_ERROR("cannot write DICOM file: " << filename);
                pResponse->DimseStatus = STATUS_STORE_Refused_OutOfResources;
                OFStandard::deleteFile(filename);
            }

            if ((pResponse->DimseStatus == STATUS_Success))
            {
                if (!DU_findSOPClassAndInstanceInDataSet(*ppImageDataSet, sopClass, sopInstance, OFTrue))
                {
                    DCMNET_FATAL("bad DICOM file: " << pImageFilename);
                    pResponse->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
                }
                else if (strcmp(sopClass, pRequest->AffectedSOPClassUID) != 0)
                {
                    pResponse->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
                }
                else if (strcmp(sopInstance, pRequest->AffectedSOPInstanceUID) != 0)
                {
                    pResponse->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
                }
            }
        }
    }
}
