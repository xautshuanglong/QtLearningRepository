#ifndef MY_WORKER_THREAD_H
#define MY_WORKER_THREAD_H

#include <QThread>

class MyWorkerThreadPool;

class MyWorkerThread : public QThread
{
    Q_OBJECT

public:
    MyWorkerThread(QObject *parent = Q_NULLPTR);
    MyWorkerThread(MyWorkerThreadPool *pThreadPool, QObject *parent = Q_NULLPTR);
    ~MyWorkerThread();

    virtual void run() override;

private:
    MyWorkerThreadPool         *mpThreadPool;
};

#endif // MY_WORKER_THREAD_H
