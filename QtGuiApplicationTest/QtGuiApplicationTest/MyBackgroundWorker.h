#ifndef MY_BACKGROUND_WORKER_H
#define MY_BACKGROUND_WORKER_H

#include <QRunnable>

/************************************************************************/
/*  ��̨�����ߣ���� QThreadPool ��ɺ�̨����ִ�С�                       */
/************************************************************************/
class MyBackgroundWorker : public QRunnable
{
public:
    MyBackgroundWorker();
    ~MyBackgroundWorker();

    /* QRunnable */
    virtual void run() override; // �̺߳���
};

#endif // MY_BACKGROUND_WORKER_H
