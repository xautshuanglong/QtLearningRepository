#include "DicomSCUFind.h"

#include <LogUtil.h>
#include <dcmtk/dcmnet/diutil.h>

static const char* gsFindModelUID[] =
{
    UID_FINDModalityWorklistInformationModel,
    UID_FINDPatientRootQueryRetrieveInformationModel,
    UID_FINDStudyRootQueryRetrieveInformationModel,
    UID_RETIRED_FINDPatientStudyOnlyQueryRetrieveInformationModel,
};


DicomSCUFind::DicomSCUFind()
    : DicomSCUBase()
{
    bzero(OFreinterpret_cast(char*, &m_request), sizeof(m_request));
}

DicomSCUFind::~DicomSCUFind()
{
}

OFCondition DicomSCUFind::PerformFind(FindModel findModel)
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
        condition = this->FindUser(gsFindModelUID[findModel], &m_overrideKeys,
                                   &m_request, &m_response,
                                   DicomSCUFind::FindUserCallback, this);
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
            LogUtil::Error(CODE_LOCATION, "DicomSCUBase::FindUser --> %s", DimseCondition::dump(errorString, condition).c_str());
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

void DicomSCUFind::HandleFindCallback(T_DIMSE_C_FindRQ *pRequest, int responseCount,
                                       T_DIMSE_C_FindRSP *pResponse, DcmDataset *pResponseIdentifiers)
{
    Q_UNUSED(pRequest);
    DCMNET_INFO("---------------------------");
    DCMNET_INFO("Find Response: " << responseCount << " (" << DU_cfindStatusString(pResponse->DimseStatus) << ")");
    DCMNET_INFO(DcmObject::PrintHelper(*pResponseIdentifiers));
}

void DicomSCUFind::FindUserCallback(void *pCallbackData, T_DIMSE_C_FindRQ *pRequest,
                                    int responseCount, T_DIMSE_C_FindRSP *pResponse, DcmDataset *pResponseIdentifiers)
{
    DicomSCUFind *pCallback = OFreinterpret_cast(DicomSCUFind*, pCallbackData);
    if (pCallback)
    {
        pCallback->HandleFindCallback(pRequest, responseCount, pResponse, pResponseIdentifiers);
    }
}
