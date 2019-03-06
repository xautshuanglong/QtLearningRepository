#pragma once

#include "DicomSCUBase.h"

class DicomSCUGet : public DicomSCUBase
{
public:
    DicomSCUGet();
    ~DicomSCUGet();

    virtual OFCondition ExcuteOperation(QSharedPointer<DicomTaskBase> &pDicomTask) override;
    virtual OFCondition PerformGet(EnumGetModel getModel) override;

    // �����ȡ��Ӧ
    void HandleGetCallback(T_DIMSE_C_GetRQ *pRequest, int responseCount,
                           T_DIMSE_C_GetRSP *pResponse, OFBool &continueFlag);
    static void GetUserCallback(void *pCallbackData, T_DIMSE_C_GetRQ *pRequest,
                                int responseCount, T_DIMSE_C_GetRSP *pResponse, OFBool &continueFlag);
    // ��ν�Ӳ���ָ���Ǵ���洢����C-GET���󱻳ɹ������������ܺ󣬻��յ�C-STORE�����Դ洢����˷��ص����ݼ�����
    void HandleSubOperationCallbackEx(T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID *presentationID, OFBool &continueSession, Uint16 &cstoreReturnStatus, DcmDataset *pRspDataset);
    static void SubOperationCallbackEx(void *pSubOpCallbackData, T_DIMSE_C_StoreRQ *request, T_ASC_PresentationContextID *presentationID,
                                       OFBool &continueSession, Uint16 &cstoreReturnStatus, DcmDataset *pRspDataset);

private:
    T_DIMSE_C_GetRQ  m_request;
    T_DIMSE_C_GetRSP m_response;
};
