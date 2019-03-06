#pragma once

#include "DicomSCUBase.h"

class DicomSCUGet : public DicomSCUBase
{
public:
    DicomSCUGet();
    ~DicomSCUGet();

    virtual OFCondition ExcuteOperation(QSharedPointer<DicomTaskBase> &pDicomTask) override;
    virtual OFCondition PerformGet(EnumGetModel getModel) override;

    // 处理获取响应
    void HandleGetCallback(T_DIMSE_C_GetRQ *pRequest, int responseCount,
                           T_DIMSE_C_GetRSP *pResponse, OFBool &continueFlag);
    static void GetUserCallback(void *pCallbackData, T_DIMSE_C_GetRQ *pRequest,
                                int responseCount, T_DIMSE_C_GetRSP *pResponse, OFBool &continueFlag);
    // 所谓子操作指的是处理存储请求（C-GET请求被成功发出并被接受后，会收到C-STORE请求，以存储服务端返回的数据集。）
    void HandleSubOperationCallbackEx(T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID *presentationID, OFBool &continueSession, Uint16 &cstoreReturnStatus, DcmDataset *pRspDataset);
    static void SubOperationCallbackEx(void *pSubOpCallbackData, T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID *presentationID,
                                       OFBool &continueSession, Uint16 &cstoreReturnStatus, DcmDataset *pRspDataset);

private:
    T_DIMSE_C_GetRQ  m_request;
    T_DIMSE_C_GetRSP m_response;
};
