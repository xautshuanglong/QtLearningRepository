#include "BackgroundWorkerTest.h"

#include <iostream>
#include <thread>
#include <functional>

#include <LogUtil.h>

namespace SL::Core
{
    BackgroundWorkerTest::BackgroundWorkerTest()
        : mRuningFlag(ATOMIC_VAR_INIT(false))
    {
    }

    BackgroundWorkerTest::~BackgroundWorkerTest()
    {
        this->Stop();
    }

    void BackgroundWorkerTest::Start()
    {
        if (!mRuningFlag)
        {
            mRuningFlag = true;
            unsigned int cpuCount = std::thread::hardware_concurrency();
            for (int i = 0; i < cpuCount; ++i)
            {
                mVectorThread.push_back(std::thread(std::bind(&BackgroundWorkerTest::ThreadFunc, this)));
            }
        }
    }

    void BackgroundWorkerTest::Stop()
    {
        std::lock_guard<std::mutex> lock(mTaskListMutex);
        mTaskList.clear();
        mRuningFlag = false;
        mConditionWorking.notify_all();
        for (int i=0; i<mVectorThread.size(); ++i)
        {
            mVectorThread[i].join();
        }
        mVectorThread.clear();
    }

    void BackgroundWorkerTest::AddTask(SPTaskInterface pTask)
    {
        std::lock_guard<std::mutex> lock(mTaskListMutex);
        mTaskList.push_back(pTask);
        mConditionWorking.notify_one();
    }

    void BackgroundWorkerTest::RemoveTask(SPTaskInterface pTask)
    {
        std::lock_guard<std::mutex> lock(mTaskListMutex);
        mTaskList.remove(pTask);
    }

    void BackgroundWorkerTest::ThreadFunc()
    {
        std::thread::id curTID = std::this_thread::get_id();
        std::shared_ptr<ITaskInterface> pTempTask = nullptr;
        bool sleepFlag = false;
        while (mRuningFlag)
        {
            pTempTask = nullptr;
            mTaskListMutex.lock();
            if (mTaskList.size() > 0)
            {
                pTempTask = mTaskList.front();
                mTaskList.pop_front();
                sleepFlag = false;
            }
            else
            {
                sleepFlag = true;
            }
            mTaskListMutex.unlock();
            if (sleepFlag || pTempTask==nullptr)
            {
                LogUtil::Debug(CODE_LOCATION, "Wait for task adding <--------", std::this_thread::get_id());
                std::unique_lock<std::mutex> lock(mConditionMutex);
                mConditionWorking.wait(lock);
            }
            else
            {
                pTempTask->DoWork();
            }

            //LogUtil::Debug(CODE_LOCATION, "tid: %d is running", std::this_thread::get_id());
        }
        std::unique_lock<std::mutex> conditionLock(mConditionMutex);
    }
}
