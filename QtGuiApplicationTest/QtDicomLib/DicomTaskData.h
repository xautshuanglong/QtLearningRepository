#pragma once

#include <QString>
#include <dcmtk/ofstd/ofstring.h>

class DcmDataset;

enum EnumExecutorType
{
    EexcutorType_UNKNOW,
    EexcutorType_ECHO,
    EexcutorType_FIND,
    EexcutorType_GET,
    EexcutorType_MOVE,
    EexcutorType_SOTRE
};

class DicomTaskBase
{
public:
    EnumExecutorType ExecutorType() { return executorType; }
    DicomTaskBase() { executorType = EexcutorType_UNKNOW; }
    virtual ~DicomTaskBase() {}

protected:
    EnumExecutorType executorType;
};

class DicomTaskEcho : public DicomTaskBase
{
public:
    ~DicomTaskEcho();
    friend class DicomTaskHelper;

private:
    DicomTaskEcho();
};

class DicomTaskFind : public DicomTaskBase
{
public:
    ~DicomTaskFind();
    friend class DicomTaskHelper;

private:
    DicomTaskFind();
};

class DicomTaskGet : public DicomTaskBase
{
public:
    ~DicomTaskGet();
    friend class DicomTaskHelper;

private:
    DicomTaskGet();
};

class DicomTaskMove : public DicomTaskBase
{
public:
    ~DicomTaskMove();
    friend class DicomTaskHelper;

private:
    DicomTaskMove();
};

class DicomTaskStore : public DicomTaskBase
{
public:
    enum StoreType
    {
        STORE_DCMFILE,  // 上传 DICOM 文件
        STORE_DATASET,  // 上传 DICOM 数据集
    };

    friend class DicomTaskHelper;
    ~DicomTaskStore();

    StoreType GetStoryType() { return m_dcmStoreType; }

    void SetFilename(const QString &filename);
    OFString& GetFilename() { return m_dcmFilename; }

    void NewDataset();
    void ClearDataset();
    DcmDataset* GetDataset() { return m_pDataset; }

private:
    DicomTaskStore();
    OFString    m_dcmFilename;
    StoreType   m_dcmStoreType;
    DcmDataset *m_pDataset;
};

class DicomTaskHelper
{
public:
    template <typename DicomTask>
    static DicomTask* NewTask() { return new DicomTask(); }

    template <typename TaskType>
    static TaskType* Convert(DicomTaskBase* pTask) { return dynamic_cast<TaskType *>(pTask); }
};
