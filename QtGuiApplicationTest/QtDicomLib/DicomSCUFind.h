#pragma once

#include "DicomSCUBase.h"

enum FindModel
{
    FindModel_Worklist,
    FindModel_PatientRoot,
    FindModel_StudyRoot,
    FindModel_PatientStudyOnly
};

class DicomSCUFind : public DicomSCUBase
{
public:
    DicomSCUFind();
    ~DicomSCUFind();

    OFCondition PerformFind(FindModel findModel);

    // 处理查询响应
    void HandleFindCallback(T_DIMSE_C_FindRQ *pRequest, int responseCount,
                            T_DIMSE_C_FindRSP *pResponse, DcmDataset *pResponseIdentifiers);
    static void FindUserCallback(void *pCallbackData, T_DIMSE_C_FindRQ *pRequest,
                                 int responseCount, T_DIMSE_C_FindRSP *pResponse, DcmDataset *pResponseIdentifiers);

private:
    T_DIMSE_C_FindRQ  m_request;
    T_DIMSE_C_FindRSP m_response;
};
