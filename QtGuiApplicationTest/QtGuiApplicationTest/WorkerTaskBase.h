#ifndef WORKER_TASK_BASE_H
#define WORKER_TASK_BASE_H

#include <QObject>
#include "IWorkerTask.h"

/************************************************************************/
/*  ������������࣬ʵ���� IWorkerTask �ӿڣ��ɱ��̳���ʵ�־��幦�ܡ�       */
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
