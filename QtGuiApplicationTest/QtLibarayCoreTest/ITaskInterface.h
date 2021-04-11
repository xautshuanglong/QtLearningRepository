#pragma once

#include "qtlibaraycoretest_global.h"

namespace SL::Core
{
    class ITaskInterface
    {
    public:
        ITaskInterface() {}
        virtual ~ITaskInterface() {}

        virtual void DoWork() = 0;
    };
}
