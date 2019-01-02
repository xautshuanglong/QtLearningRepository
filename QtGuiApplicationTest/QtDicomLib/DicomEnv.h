#pragma once

#include "qtdicomlib_global.h"

class QTDICOMLIB_EXPORT DicomEnv
{
public:
    DicomEnv();
    ~DicomEnv();
    static void Initialize();
    static void Uninitialize();

private:
    static void InitializeNetwork();
    static void UninitializeNetwork();
    static void InitializeLog4Cplus();

private:
    static bool isNetworkInitialized;
    static bool isLog4CplusInitialized;
};
