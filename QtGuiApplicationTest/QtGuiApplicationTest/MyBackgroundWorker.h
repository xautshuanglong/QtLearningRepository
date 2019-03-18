#ifndef MY_BACKGROUND_WORKER_H
#define MY_BACKGROUND_WORKER_H

#include <QRunnable>
#include <QThreadStorage>
#include <QCache>

/************************************************************************/
/*  ��̨�����ߣ���� QThreadPool ��ɺ�̨����ִ�С�                       */
/*  activeThreadCount : �̳߳��л���߳�������                          */
/*  expiryTimeout : ���ڵ��̻߳��Զ��˳���Ĭ�� 30 �룬                    */
/*                  ���ָ��Ϊ��ֵ�����������ڣ�ֻӰ���´������̣߳�       */
/*                  ���鴴�� ThreadPool �󣬵��� Start ǰ���ø�ֵ��       */
/*  maxThreadCount : �߳�������߳�������                                */
/************************************************************************/
class MyBackgroundWorker : public QRunnable
{
public:
    MyBackgroundWorker();
    ~MyBackgroundWorker();

    /* QRunnable */
    virtual void run() override; // �̺߳���

private:
    static QThreadStorage<QCache<int, int>>  m_ThreadLocalTest;
};

#endif // MY_BACKGROUND_WORKER_H
