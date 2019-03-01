#pragma once

#include "DicomSCUBase.h"

class DcmDataset;

class DicomSCUStore : public DicomSCUBase
{
public:
    DicomSCUStore();
    ~DicomSCUStore();

    virtual OFCondition PerformStore(DcmDataset *pStoreDataset) override;
    OFCondition PerformStore(OFString& filename);

    // ¥¶¿Ì¥Ê¥¢œÏ”¶
    void HandleStoreCallback(T_DIMSE_StoreProgress *pProgress, T_DIMSE_C_StoreRQ *pRequest);
    static void StoreUserCallback(void *pCallbackData, T_DIMSE_StoreProgress *pProgress, T_DIMSE_C_StoreRQ *pRequest);

private:
    T_DIMSE_C_StoreRQ  m_request;
    T_DIMSE_C_StoreRSP m_response;
};
