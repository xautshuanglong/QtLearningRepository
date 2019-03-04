#pragma once

#include "DicomSCUBase.h"

class DicomSCUMove : public DicomSCUBase
{
public:
    DicomSCUMove();
    ~DicomSCUMove();

    virtual OFCondition ExcuteOperation() override;
    virtual OFCondition PerformMove(EnumMoveModel moveModel) override;

    // 处理转储响应
    static void MoveUserCallback(void *pCallbackData, T_DIMSE_C_MoveRQ *pRequest, int responseCount, T_DIMSE_C_MoveRSP *pResponse);
    static void SubOpProviderCallback(void *pSubOpCallbackData, T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc);

private:
    void HandleMoveCallback(T_DIMSE_C_MoveRQ *pRequest, int responseCount, T_DIMSE_C_MoveRSP *pResponse);
    void HandleSubOperationCallback(T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc);

    void AcceptSubAssociation(T_ASC_Network *pNetwork, T_ASC_Association **ppSubOpAssoc);
    void SubOperation_SCP(T_ASC_Association **ppSubOpAssoc);
    OFCondition SubOperation_EchoSCP(T_ASC_Association *pSubOpAssociation, T_DIMSE_Message *pReceivedMsg, T_ASC_PresentationContextID presentationID);
    OFCondition SubOperation_StoreSCP(T_ASC_Association *pSubOpAssociation, T_DIMSE_Message *pReceivedMsg, T_ASC_PresentationContextID presentationID);
    static void StoreProviderCallback(void *pCallbackData, T_DIMSE_StoreProgress *pProgress, T_DIMSE_C_StoreRQ *pRequest,
                                      char *pImageFilename, DcmDataset **ppImageDataSet, T_DIMSE_C_StoreRSP *pResponse,
                                      DcmDataset **ppStatusDetail);

private:
    T_DIMSE_C_MoveRQ  m_request;
    T_DIMSE_C_MoveRSP m_response;
};
