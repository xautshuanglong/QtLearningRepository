#pragma once

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

class DicomTaskFind : public DicomTaskBase
{
    friend class DicomTaskHelper;
private:
    DicomTaskFind() { executorType = EexcutorType_FIND; }
};

class DicomTaskGet : public DicomTaskBase
{
    friend class DicomTaskHelper;
private:
    DicomTaskGet() { executorType = EexcutorType_GET; }
};

class DicomTaskMove : public DicomTaskBase
{
    friend class DicomTaskHelper;
private:
    DicomTaskMove() { executorType = EexcutorType_MOVE; }
};

class DicomTaskStore : public DicomTaskBase
{
    friend class DicomTaskHelper;
private:
    DicomTaskStore() { executorType = EexcutorType_SOTRE; }
};

class DicomTaskHelper
{
public:
    template <typename DicomTask>
    static DicomTask* NewTask() { return new DicomTask(); }

    template <typename TaskType>
    static TaskType* ConvertTo() { return dynamic_cast<TaskType *>(this); }
};
