#include "TaskBase.h"

#include <iostream>
#include <thread>

#include <LogUtil.h>

namespace SL::Core
{
    thread_local static int gsThreadLocalTest = 0;

    TaskBase::TaskBase()
        : mnCount(0)
    {
    }

    TaskBase::~TaskBase()
    {
    }

    void TaskBase::DoWork()
    {
        while (mnCount < 100)
        {
            ++mnCount;
            LogUtil::Debug(CODE_LOCATION, "this: 0x%08X  count=%d threadlocal=%d", this, mnCount, gsThreadLocalTest++);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
