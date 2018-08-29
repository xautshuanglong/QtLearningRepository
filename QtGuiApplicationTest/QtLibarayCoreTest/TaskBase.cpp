#include "TaskBase.h"

#include <iostream>
#include <thread>

#include <LogUtil.h>

namespace SL::Core
{
    TaskBase::TaskBase()
        : mnCount(0)
    {
    }

    TaskBase::~TaskBase()
    {
    }

    void TaskBase::DoWork()
    {
        while (mnCount < 300)
        {
            ++mnCount;
            LogUtil::Debug(CODE_LOCATION, "this: 0x%08X  count=%d", this, mnCount);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
