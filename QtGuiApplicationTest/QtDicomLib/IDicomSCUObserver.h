#pragma once

#include "qtdicomlib_global.h"

class QTDICOMLIB_EXPORT IDicomSCUObserver
{
public:
    IDicomSCUObserver() {}
    virtual ~IDicomSCUObserver() {}

    virtual void HandleResponseEcho() = 0;
    virtual void HandleResponseFind() = 0;
    virtual void HandleResponseGet() = 0;
    virtual void HandleResponseMove() = 0;
    virtual void HandleResponseStore() = 0;
};
