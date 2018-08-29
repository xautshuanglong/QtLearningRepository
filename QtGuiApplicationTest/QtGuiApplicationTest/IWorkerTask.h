#ifndef I_WORKER_TASK_H
#define I_WORKER_TASK_H

/************************************************************************/
/*  定义任务执行入口，配合后天工作者线程池使用。                           */
/*  工作者要执行的任务必须实现该抽象类中的纯虚函数。                       */
/*  （可通过 WorkerTaskBase 达到这一目的）                               */
/************************************************************************/
class IWorkerTask
{
public:
    IWorkerTask::IWorkerTask() {}
    virtual IWorkerTask::~IWorkerTask() {}

    virtual void DoWorkerTask() = 0;
};

#endif // I_WORKER_TASK_H
