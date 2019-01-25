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
    void HandleGetCallback(T_DIMSE_C_GetRQ *pRequest, int responseCount,
                           T_DIMSE_C_GetRSP *pResponse, OFBool &continueFlag);
    static void GetUserCallback(void *pCallbackData, T_DIMSE_C_GetRQ *pRequest,
                                int responseCount, T_DIMSE_C_GetRSP *pResponse, OFBool &continueFlag);
    // ����洢��Ӧ��C-GET���󱻴������յ�C-STORE���󣬴洢����˷��ص����ݼ�����
    void HandleSubOperationCallbackEx(T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID presentationID, Uint16 &continueSession);
    static void SubOperationCallbackEx(void *pSubOpCallbackData, T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID presentationID, Uint16 &continueSession);

private:
    T_DIMSE_C_GetRQ  m_request;
    T_DIMSE_C_GetRSP m_response;
};
