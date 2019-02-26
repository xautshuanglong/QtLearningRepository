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
    static void ConfigureLog4CplusFormFile(); // ͨ���ļ�������־���ߣ������޸ĵȼ��������־���Լ�������
    static void ConfigureLog4CplusFormCode(); // Ӳ�������ã���ֹ�û��۸���־���������ļ�

private:
    static bool isNetworkInitialized;
    static bool isLog4CplusInitialized;
};
