#include "DicomSCUGet.h"

#include <LogUtil.h>
#include <dcmtk/dcmnet/diutil.h>

static const char* gsGetModelUID[] =
{
    UID_GETPatientRootQueryRetrieveInformationModel,
    UID_GETStudyRootQueryRetrieveInformationModel,
    UID_RETIRED_GETPatientStudyOnlyQueryRetrieveInformationModel,
};

DicomSCUGet::DicomSCUGet()
    : DicomSCUBase()
{
}

DicomSCUGet::~DicomSCUGet()
{
}

OFCondition DicomSCUGet::PerformGet(GetModel getModel)
{
    OFCondition condition;

    condition = this->InitNetwork();
    if (condition.good())
    {
        condition = this->NegotiateAssociation();
        // TODO 处理关联协商结果
    }
    if (condition.good())
    {
        condition = this->GetUser(gsGetModelUID[getModel], &m_overrideKeys,
                                  &m_request, &m_response,
                                  DicomSCUGet::GetUserCallback, this,
                                  DicomSCUGet::SubOperationCallbackEx, this);
        if (condition.bad())
        {
            OFString errorString;
            LogUtil::Error(CODE_LOCATION, "DicomSCUBase::GetUser --> %s", DimseCondition::dump(errorString, condition).c_str());
        }
        // TODO 处理响应消息
    }

    condition = this->DropNetwork();
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "DicomSCUBase::DropNetwork --> %s", DimseCondition::dump(errorString, condition).c_str());
    }

    return condition;
}

void DicomSCUGet::HandleGetCallback(T_DIMSE_C_GetRQ *pRequest, int responseCount, T_DIMSE_C_GetRSP *pResponse, OFBool &continueFlag)
{
    DCMNET_INFO("---------------------------");
    DCMNET_INFO("Handling C-GET Response: " << responseCount << " (" << DU_cfindStatusString(pResponse->DimseStatus) << ")");

    
    Uint16 highNibble = pResponse->DimseStatus & 0xf000;
    if (highNibble == STATUS_GET_Failed_UnableToProcess)
    {
        continueFlag = OFFalse;
        DCMNET_ERROR("Unable to Process");
        DCMNET_WARN("Full status is 0x" << std::hex << std::setfill('0') << std::setw(4) << pResponse->DimseStatus);
    }

    switch (pResponse->DimseStatus)
    {
    case STATUS_GET_Refused_OutOfResourcesNumberOfMatches:
        continueFlag = OFFalse;
        DCMNET_ERROR("Out of Resources - Unable to calculate number of matches");
        break;
    case STATUS_GET_Refused_OutOfResourcesSubOperations:
        continueFlag = OFFalse;
        DCMNET_ERROR("Out of Resources - Unable to perform sub-operations");
        break;
    case STATUS_GET_Failed_IdentifierDoesNotMatchSOPClass:
        continueFlag = OFFalse;
        DCMNET_ERROR("Identifier does not match SOP class");
        break;
    case STATUS_GET_Cancel_SubOperationsTerminatedDueToCancelIndication:
        continueFlag = OFFalse;
        DCMNET_DEBUG("Suboperations canceled by server due to CANCEL indication");
        break;
    case STATUS_GET_Warning_SubOperationsCompleteOneOrMoreFailures:
        continueFlag = OFFalse;
        DCMNET_WARN("Suboperations of C-GET completed with one or more failures");
        break;
    case STATUS_Pending:
        continueFlag = OFTrue;
        DCMNET_DEBUG("One or more pending C-GET responses");
        break;
    case STATUS_Success:
        continueFlag = OFFalse;
        DCMNET_DEBUG("Received final C-GET response, no more C-GET responses expected");
        break;
    default:
        continueFlag = OFFalse;
        DCMNET_WARN("Status is 0x" << std::hex << std::setfill('0') << std::setw(4)
                    << pResponse->DimseStatus << " (unknown)");
        DCMNET_WARN("Will not wait for further C-GET responses");
        break;
    }
}

void DicomSCUGet::GetUserCallback(void *pCallbackData, T_DIMSE_C_GetRQ *pRequest,
                                   int responseCount, T_DIMSE_C_GetRSP *pResponse, OFBool &continueFlag)
{
    DicomSCUGet *pCallback = OFreinterpret_cast(DicomSCUGet*, pCallbackData);
    if (pCallback)
    {
        pCallback->HandleGetCallback(pRequest, responseCount, pResponse, continueFlag);
    }
}

void DicomSCUGet::HandleSubOperationCallbackEx(T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID presentationID, Uint16 &continueSession)
{
    ;
}

void DicomSCUGet::SubOperationCallbackEx(void *pSubOpCallbackData, T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID presentationID, Uint16 &continueSession)
{
    // TODO 处理存储请求
    DicomSCUGet *pCallback = OFreinterpret_cast(DicomSCUGet*, pSubOpCallbackData);
    if (pCallback)
    {
        pCallback->HandleSubOperationCallbackEx(request, presentationID, continueSession);
    }
}
