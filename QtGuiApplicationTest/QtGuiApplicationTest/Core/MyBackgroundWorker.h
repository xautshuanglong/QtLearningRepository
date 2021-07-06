#ifndef MY_BACKGROUND_WORKER_H
#define MY_BACKGROUND_WORKER_H

#include <QRunnable>
#include <QThreadStorage>
#include <QCache>

/************************************************************************/
/*  后台工作者，配合 QThreadPool 完成后台任务执行。                       */
/*  activeThreadCount : 线程池中活动的线程数量。                          */
/*  expiryTimeout : 过期的线程会自动退出，默认 30 秒，                    */
/*                  如果指定为负值，将永不过期，只影响新创建的线程，       */
/*                  建议创建 ThreadPool 后，调用 Start 前设置该值。       */
/*  maxThreadCount : 线程中最大线程数量。                                */
/************************************************************************/
class MyBackgroundWorker : public QRunnable
{
public:
    MyBackgroundWorker();
    ~MyBackgroundWorker();

    /* QRunnable */
    virtual void run() override; // 线程函数

private:
    static QThreadStorage<QCache<int, int>>  m_ThreadLocalTest;
};

#endif // MY_BACKGROUND_WORKER_H
