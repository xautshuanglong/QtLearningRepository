#pragma once

#include "qtlibaraycoretest_global.h"

namespace SL
{
    namespace Core
    {
        class QTLIBARAYCORETEST_EXPORT ITaskInterface
        {
        public:
            ITaskInterface::ITaskInterface() {}
            virtual ITaskInterface::~ITaskInterface() {}

            virtual void DoWork() = 0;
        };
    }
}
