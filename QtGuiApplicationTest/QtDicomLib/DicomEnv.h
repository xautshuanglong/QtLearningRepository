#pragma once

#include "qtdicomlib_global.h"

class QTDICOMLIB_EXPORT DicomEnv
{
public:
    ~DicomEnv();
    static void Initialize();
    static void Uninitialize();

private:
    DicomEnv();
    static void InitializeNetwork();
    static void UninitializeNetwork();
    static void InitializeLog4Cplus();
    static void ConfigureLog4CplusFormFile(); // 通过文件配置日志工具，方便修改等级，添加日志策略及过滤器
    static void ConfigureLog4CplusFormCode(); // 硬编码配置，防止用户篡改日志工具配置文件

private:
    static bool isNetworkInitialized;
    static bool isLog4CplusInitialized;
};
