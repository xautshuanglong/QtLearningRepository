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
    // 处理获取响应
    void HandleGetCallback(T_DIMSE_C_GetRQ *pRequest, int responseCount, T_DIMSE_C_GetRSP *pResponse);
    static void GetUserCallback(void *pCallbackData, T_DIMSE_C_GetRQ *pRequest,
                                int responseCount, T_DIMSE_C_GetRSP *pResponse);
    // 处理存储响应（C-GET请求被处理后会收到C-STORE请求，存储服务端返回的数据集。）
    void HandleSubOperationStore(T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc);
    static void SubOperationCallback(void *pSubOpCallbackData, T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc);

private:
    T_DIMSE_C_GetRQ  m_request;
    T_DIMSE_C_GetRSP m_response;
};
