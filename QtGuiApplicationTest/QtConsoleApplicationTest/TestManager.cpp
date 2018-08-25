#include "TestManager.h"

#include "DicomFileParserTest.h"
#include "CognexDataManSDKTest.h"
#include "ImportComTest.h"

TestManager::TestManager()
{
    mpDicomFileParserTest = new DicomFileParserTest();
    mpCognexDataManSDKTest = new CognexDataManSDKTest();
    mpImportComTest = new ImportComTest();
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
    //mpDicomFileParserTest->TestEnter();
    //mpCognexDataManSDKTest->TestEnter();
    mpImportComTest->TestEnter();
}

void TestManager::Exit()
{
    mpDicomFileParserTest->TestExit();
    mpCognexDataManSDKTest->TestExit();
    mpImportComTest->TestExit();
}
