#include "TestManager.h"

#include "DicomFileParserTest.h"
#include "CognexDataManSDKTest.h"

TestManager::TestManager()
{
    mpDicomFileParserTest = new DicomFileParserTest();
    mpCognexDataManSDKTest = new CognexDataManSDKTest();
}

TestManager::~TestManager()
{
}

TestManager* TestManager::Instance()
{
    static TestManager instance;
    return &instance;
}

void TestManager::Enter()
{
    mpDicomFileParserTest->TestEnter();
    mpCognexDataManSDKTest->TestEnter();
}

void TestManager::Exit()
{
    mpDicomFileParserTest->TestExit();
    mpCognexDataManSDKTest->TestExit();
}
