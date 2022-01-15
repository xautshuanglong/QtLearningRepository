#ifndef BACKGROUND_WORKER_TEST_H
#define BACKGROUND_WORKER_TEST_H

#include "qtlibrarycoretest_global.h"

#include <atomic>
#include <list>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "ITaskInterface.h"

namespace SL::Core
{
    /************************************************************************/
    /*    在后台处理耗时任务                                                 */
    /************************************************************************/
    class QTLIBRARYCORETEST_EXPORT BackgroundWorkerTest
    {
        typedef std::shared_ptr<ITaskInterface>            SPTaskInterface;
        typedef std::list<std::shared_ptr<ITaskInterface>> SPTaskList;
        typedef std::vector<std::thread>                   ThreadVector;
    public:
        BackgroundWorkerTest();
        ~BackgroundWorkerTest();

        void Start();
        void Stop();
        void AddTask(SPTaskInterface pTask);
        void RemoveTask(SPTaskInterface pTask);

    protected:
        void ThreadFunc();

    private:
        std::atomic<bool>        mRuningFlag;
        ThreadVector             mVectorThread;
        SPTaskList               mTaskList;
        std::mutex               mTaskListMutex;    // 互斥后台任务的增加与删除操作
        std::mutex               mConditionMutex;   // 配合条件变量，是否阻塞
        std::condition_variable  mConditionWorking; // 开始执行任务
    };
}

#endif // BACKGROUND_WORKER_TEST_H
