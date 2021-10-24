#include "BackgroundWorkerTest.h"

#include <iostream>
#include <thread>
#include <functional>

#include <Windows.h>

#include "JCB_Logger/LogUtil.h"

// 调试器中线程命名测试
const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

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
                //mVectorThread.push_back(std::thread(std::bind(&BackgroundWorkerTest::ThreadFunc, this))); // 替换如下

                std::thread workThread(std::bind(&BackgroundWorkerTest::ThreadFunc, this));
                std::thread::id tid = workThread.get_id();
                DWORD dwThreadID = ::GetThreadId(static_cast<HANDLE>(workThread.native_handle()));
                mVectorThread.push_back(std::forward<std::thread>(workThread));
                std::string strThreadName = "Shuanglong test " + std::to_string(i);

                THREADNAME_INFO info;
                info.dwType = 0x1000;
                info.szName = strThreadName.c_str();
                info.dwThreadID = dwThreadID;
                info.dwFlags = 0;

                RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
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
