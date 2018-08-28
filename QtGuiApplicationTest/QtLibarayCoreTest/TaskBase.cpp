#include "TaskBase.h"

#include <iostream>
#include <thread>

#include <LogUtil.h>

namespace SL::Core
{
    TaskBase::TaskBase()
    {
    }

    TaskBase::~TaskBase()
    {
    }

    void TaskBase::DoWork()
    {
        static int count = 0;
        while (count < 4000)
        {
            ++count;
            LogUtil::Debug(CODE_LOCATION, "tid: %d  count=%d", std::this_thread::get_id(), count);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
