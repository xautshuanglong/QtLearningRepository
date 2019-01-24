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
                                  DicomSCUGet::SubOperationCallback, this);
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

void DicomSCUGet::HandleGetCallback(T_DIMSE_C_GetRQ *pRequest, int responseCount, T_DIMSE_C_GetRSP *pResponse)
{
    DCMNET_INFO("---------------------------");
    DCMNET_INFO("Get Response: " << responseCount << " (" << DU_cfindStatusString(pResponse->DimseStatus) << ")");
    //    DCMNET_INFO(DcmObject::PrintHelper(*pResponseIdentifiers));
}

void DicomSCUGet::GetUserCallback(void *pCallbackData, T_DIMSE_C_GetRQ *pRequest,
                                   int responseCount, T_DIMSE_C_GetRSP *pResponse)
{
    DicomSCUGet *pCallback = OFreinterpret_cast(DicomSCUGet*, pCallbackData);
    if (pCallback)
    {
        pCallback->HandleGetCallback(pRequest, responseCount, pResponse);
    }
}

void DicomSCUGet::HandleSubOperationStore(T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc)
{
    ;
}

void DicomSCUGet::SubOperationCallback(void *pSubOpCallbackData, T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc)
{
    // TODO 处理存储请求
    DicomSCUGet *pCallback = OFreinterpret_cast(DicomSCUGet*, pSubOpCallbackData);
    if (pCallback)
    {
        pCallback->HandleSubOperationStore(pNetwork, ppSubOpAssoc);
    }
}

