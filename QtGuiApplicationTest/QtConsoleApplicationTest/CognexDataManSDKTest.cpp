#include "CognexDataManSDKTest.h"

#include <CognexDataManSDK.h>

CognexDataManSDKTest::CognexDataManSDKTest()
{
}

CognexDataManSDKTest::~CognexDataManSDKTest()
{
}

void CognexDataManSDKTest::TestEnter()
{
    this->PrintCLR();
}

void CognexDataManSDKTest::TestExit()
{
    ;
}

void CognexDataManSDKTest::PrintCLR()
{
    CognexDataManSDK::TestAPI testAPI;
    testAPI.PrintTest();
}
