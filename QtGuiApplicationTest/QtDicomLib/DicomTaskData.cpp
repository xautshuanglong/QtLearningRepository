#include "DicomTaskData.h"

#include <dcmtk/dcmdata/dcdatset.h>
#include "JCB_Logger/LogUtil.h"

/************************************************************************/
/* C-ECHO                                                               */
/************************************************************************/
DicomTaskEcho::DicomTaskEcho()
{
    executorType = EexcutorType_ECHO;
}

DicomTaskEcho::~DicomTaskEcho()
{
}

/************************************************************************/
/* C-FIND                                                               */
/************************************************************************/
DicomTaskFind::DicomTaskFind()
{
    executorType = EexcutorType_FIND;
}

DicomTaskFind::~DicomTaskFind()
{
}

/************************************************************************/
/* C-GET                                                                */
/************************************************************************/
DicomTaskGet::DicomTaskGet()
{
    executorType = EexcutorType_GET;
}

DicomTaskGet::~DicomTaskGet()
{
}

/************************************************************************/
/* C-MOVE                                                               */
/************************************************************************/
DicomTaskMove::DicomTaskMove()
{
    executorType = EexcutorType_MOVE;
}

DicomTaskMove::~DicomTaskMove()
{
}

/************************************************************************/
/* C-STORE                                                              */
/************************************************************************/
DicomTaskStore::DicomTaskStore()
    : m_pDataset(Q_NULLPTR)
{
    executorType = EexcutorType_SOTRE;
    LogUtil::Debug(CODE_LOCATION, "new Task DicomTaskStore::DicomTaskStore()");
}

DicomTaskStore::~DicomTaskStore()
{
    this->ClearDataset();
    LogUtil::Debug(CODE_LOCATION, "new Task DicomTaskStore::~DicomTaskStore()");
}

void DicomTaskStore::SetFilename(const QString &filename)
{
    m_dcmStoreType = STORE_DCMFILE;
    m_dcmFilename = OFString(filename.toStdString().c_str());
}

void DicomTaskStore::NewDataset()
{
    m_dcmStoreType = STORE_DATASET;
    this->ClearDataset();
    m_pDataset = new DcmDataset();
}

void DicomTaskStore::ClearDataset()
{
    if (m_pDataset != Q_NULLPTR)
    {
        delete m_pDataset;
        m_pDataset = Q_NULLPTR;
    }
}
