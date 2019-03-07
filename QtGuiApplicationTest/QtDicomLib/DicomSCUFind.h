#pragma once

#include "DicomSCUBase.h"

enum EnumFindModel
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

    OFCondition PerformFind(EnumFindModel findModel);
    virtual OFCondition ExcuteOperation(QSharedPointer<DicomTaskBase> &pDicomTask) override;

    // �����ѯ��Ӧ
    void HandleFindCallback(T_DIMSE_C_FindRQ *pRequest, int responseCount,
                            T_DIMSE_C_FindRSP *pResponse, DcmDataset *pResponseIdentifiers);
    static void FindUserCallback(void *pCallbackData, T_DIMSE_C_FindRQ *pRequest,
                                 int responseCount, T_DIMSE_C_FindRSP *pResponse, DcmDataset *pResponseIdentifiers);

private:
    T_DIMSE_C_FindRQ  m_request;
    T_DIMSE_C_FindRSP m_response;
};
