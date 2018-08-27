#include "CognexDataManSDKTest.h"

#include <DataManManager.h>

CognexDataManSDKTest::CognexDataManSDKTest()
    : mpDataManManager(nullptr)
{
}

CognexDataManSDKTest::~CognexDataManSDKTest()
{
}

void CognexDataManSDKTest::TestEnter()
{
    this->DataManManagerTest();
}

void CognexDataManSDKTest::TestExit()
{
    if (mpDataManManager != nullptr)
    {
        delete mpDataManManager;
        mpDataManManager = nullptr;
    }
}

void CognexDataManSDKTest::DataManManagerTest()
{
    mpDataManManager = new CognexDataManSDK::DataManManager();
    mpDataManManager->Discover();
}
