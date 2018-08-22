#include "CognexDataManSDKTest.h"

#include <DataManManager.h>

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
    CognexDataManSDK::DataManManager test;
    test.Discover();
}
