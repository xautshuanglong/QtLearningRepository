#ifndef I_WORKER_TASK_H
#define I_WORKER_TASK_H

/************************************************************************/
/*  ��������ִ����ڣ���Ϻ��칤�����̳߳�ʹ�á�                           */
/*  ������Ҫִ�е��������ʵ�ָó������еĴ��麯����                       */
/*  ����ͨ�� WorkerTaskBase �ﵽ��һĿ�ģ�                               */
/************************************************************************/
class IWorkerTask
{
public:
    IWorkerTask::IWorkerTask() {}
    virtual IWorkerTask::~IWorkerTask() {}

    virtual void DoWorkerTask() = 0;
};

#endif // I_WORKER_TASK_H
