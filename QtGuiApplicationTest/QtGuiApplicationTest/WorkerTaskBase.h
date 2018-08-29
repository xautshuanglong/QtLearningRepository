#ifndef WORKER_TASK_BASE_H
#define WORKER_TASK_BASE_H

#include <QObject>
#include "IWorkerTask.h"

/************************************************************************/
/*  工作者任务基类，实现了 IWorkerTask 接口，可被继承以实现具体功能。       */
/************************************************************************/
class WorkerTaskBase : public QObject, public IWorkerTask
{
    Q_OBJECT
public:
    WorkerTaskBase(QObject *parent = Q_NULLPTR);
    ~WorkerTaskBase();

    /* IWorkerTask */
    virtual void DoWorkerTask() override;
};

#endif // WORKER_TASK_BASE_H
