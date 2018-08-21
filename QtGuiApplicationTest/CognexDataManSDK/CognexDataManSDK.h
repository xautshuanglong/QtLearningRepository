// CognexDataManSDK.h

#pragma once

#include <iostream>

#ifndef COGNEX_EXPORT
#if defined(COGNEX_SHARED_DLL)
#define COGNEX_EXPORT __declspec(dllexport)
#else
#define COGNEX_EXPORT __declspec(dllimport)
#endif
#endif

namespace CognexDataManSDK
{
    class COGNEX_EXPORT TestAPI
    {
    public:
        TestAPI();
        ~TestAPI();

        void PrintTest();
    };
}
