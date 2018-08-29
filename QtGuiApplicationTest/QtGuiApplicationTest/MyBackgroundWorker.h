#ifndef MY_BACKGROUND_WORKER_H
#define MY_BACKGROUND_WORKER_H

#include <QRunnable>

/************************************************************************/
/*  后台工作者，配合 QThreadPool 完成后台任务执行。                       */
/************************************************************************/
class MyBackgroundWorker : public QRunnable
{
public:
    MyBackgroundWorker();
    ~MyBackgroundWorker();

    /* QRunnable */
    virtual void run() override; // 线程函数
};

#endif // MY_BACKGROUND_WORKER_H
