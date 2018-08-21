// This is the main DLL file.

#include "stdafx.h"

#include "CognexDataManSDK.h"

using namespace System;

namespace CognexDataManSDK
{
    TestAPI::TestAPI()
    {
        ;
    }

    TestAPI::~TestAPI()
    {
        ;
    }

    void TestAPI::PrintTest()
    {
        std::cout << "abc" << std::endl;
        //System::Console.Out.Write("Hello, world!");
        Console::Write("Hello, world");
    }
}
