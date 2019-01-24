#pragma once

#include "DicomSCUBase.h"

enum GetModel
{
    GetModel_PatientRoot,
    GetModel_StudyRoot,
    GetModel_PatientStudyOnly
};

class QTDICOMLIB_EXPORT DicomSCUGet : public DicomSCUBase
{
public:
    DicomSCUGet();
    ~DicomSCUGet();

    OFCondition PerformGet(GetModel getModel);
    // �����ȡ��Ӧ
    void HandleGetCallback(T_DIMSE_C_GetRQ *pRequest, int responseCount, T_DIMSE_C_GetRSP *pResponse);
    static void GetUserCallback(void *pCallbackData, T_DIMSE_C_GetRQ *pRequest,
                                int responseCount, T_DIMSE_C_GetRSP *pResponse);
    // ����洢��Ӧ��C-GET���󱻴������յ�C-STORE���󣬴洢����˷��ص����ݼ�����
    void HandleSubOperationStore(T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc);
    static void SubOperationCallback(void *pSubOpCallbackData, T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc);

private:
    T_DIMSE_C_GetRQ  m_request;
    T_DIMSE_C_GetRSP m_response;
};
