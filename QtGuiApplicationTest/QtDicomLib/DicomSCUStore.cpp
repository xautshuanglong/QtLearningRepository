#include "DicomSCUStore.h"

// DCMTK Headers
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmnet/diutil.h>

// Self Headers
#include <LogUtil.h>

DicomSCUStore::DicomSCUStore()
    : DicomSCUBase()
{
    bzero(OFreinterpret_cast(char*, &m_request), sizeof(m_request));
}

DicomSCUStore::~DicomSCUStore()
{
}

OFCondition DicomSCUStore::ExcuteOperation(QSharedPointer<DicomTaskBase> &pDicomTask)
{
    OFCondition condition = EC_IllegalCall;
    DicomTaskStore *pTaskStore = DicomTaskHelper::Convert<DicomTaskStore>(pDicomTask.data());
    if (pTaskStore != Q_NULLPTR)
    {
        switch (pTaskStore->GetStoryType())
        {
        case DicomTaskStore::STORE_DCMFILE:
            this->PerformStore(pTaskStore->GetFilename());
            break;
        case DicomTaskStore::STORE_DATASET:
            this->PerformStore(pTaskStore->GetDataset());
            break;
        default:
            break;
        }
    }
    return condition;
    //OFCondition condition = m_pDicomStore->PerformStore(filename);
    //if (condition.bad())
    //{
    //    OFString errorString;
    //    LogUtil::Error(CODE_LOCATION, "PerformStore Error: %s", DimseCondition::dump(errorString, condition).c_str());
    //    // TODO 向业务层报告错误
    //}
    return EC_NotYetImplemented;
}

OFCondition DicomSCUStore::PerformStore(OFString& filename)
{
    OFCondition condition = EC_Normal;
    DcmFileFormat dcmFileFormat;
    OFBool foundFlag = OFFalse;
    char sopClassUID[128];
    char sopInstanceUID[128];

    if (OFStandard::fileExists(filename))
    {
        condition = dcmFileFormat.loadFile(filename);
        if (condition.good())
        {
            foundFlag = DU_findSOPClassAndInstanceInDataSet(dcmFileFormat.getMetaInfo(), sopClassUID, sopInstanceUID, OFFalse);
            if (!foundFlag)
            {
                foundFlag = DU_findSOPClassAndInstanceInDataSet(dcmFileFormat.getDataset(), sopClassUID, sopInstanceUID, OFFalse);
            }
            if (!foundFlag)
            {
                condition = DIMSE_BADDATA;
            }
        }
        else
        {
            DCMNET_ERROR("Bad DICOM file: " << filename << " --> " << condition.text());
            condition = DIMSE_BADDATA;
        }
    }
    else
    {
        DCMNET_DEBUG("Can not find file: " << filename);
        condition = EC_InvalidFilename;
    }

    if (condition.good())
    {
        condition = this->PerformStore(dcmFileFormat.getDataset());
    }

    return condition;
}

OFCondition DicomSCUStore::PerformStore(DcmDataset *pStoreDataset)
{
    OFCondition condition;

    condition = this->InitNetwork();
    if (condition.good())
    {
        this->UseSecureConnection();
    }
    if (condition.good())
    {
        condition = this->NegotiateAssociation();
        // TODO 处理关联协商结果
    }
    if (condition.good())
    {
        condition = this->StoreUser(pStoreDataset, &m_request, &m_response,
                                    DicomSCUStore::StoreUserCallback, this);
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
            LogUtil::Error(CODE_LOCATION, "DicomSCUBase::StoreUser --> %s", DimseCondition::dump(errorString, condition).c_str());
        }
    }

    condition = this->DropNetwork();
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "DicomSCUBase::DropNetwork --> %s", DimseCondition::dump(errorString, condition).c_str());
    }

    return condition;
}

void DicomSCUStore::HandleStoreCallback(T_DIMSE_StoreProgress *pProgress, T_DIMSE_C_StoreRQ *pRequest)
{
    OFString tempString;

    if (pProgress->state == DIMSE_StoreBegin)
    {
        DCMNET_DEBUG(DIMSE_dumpMessage(tempString, *pRequest, DIMSE_OUTGOING));
    }

    switch (pProgress->state)
    {
    case DIMSE_StoreBegin:
        DCMNET_DEBUG("StoreBegin ========================");
        break;
    case DIMSE_StoreProgressing:
        //DCMNET_DEBUG("StoreProgressing (" << pProgress->callbackCount << ") : "
        //             << pProgress->progressBytes << "/" << pProgress->totalBytes);
        break;
    case DIMSE_StoreEnd:
        DCMNET_DEBUG("StoreEnd ========================");
        break;
    default:
        break;
    }
}

void DicomSCUStore::StoreUserCallback(void *pCallbackData, T_DIMSE_StoreProgress *pProgress, T_DIMSE_C_StoreRQ *pRequest)
{
    DicomSCUStore *pCallback = OFreinterpret_cast(DicomSCUStore*, pCallbackData);
    if (pCallback)
    {
        pCallback->HandleStoreCallback(pProgress, pRequest);
    }
}
