#ifndef TASK_BASE_H
#define TASK_BASE_H

#include "qtlibaraycoretest_global.h"
#include "ITaskInterface.h"

namespace SL
{
    namespace Core
    {
        class QTLIBARAYCORETEST_EXPORT TaskBase : public ITaskInterface
        {
        public:
            TaskBase();
            ~TaskBase();

            /* ITaskInterface */
            virtual void DoWork() override;
        };
    }
}

#endif // TASK_BASE_H
