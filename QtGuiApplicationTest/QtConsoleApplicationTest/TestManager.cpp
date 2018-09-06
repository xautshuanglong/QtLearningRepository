#include "TestManager.h"

#include "DicomFileParserTest.h"
#include "CognexDataManSDKTest.h"
#include "ImportComTest.h"
#include "OpensslTest.h"

TestManager::TestManager()
{
    mpDicomFileParserTest = new DicomFileParserTest();
    mpCognexDataManSDKTest = new CognexDataManSDKTest();
    mpImportComTest = new ImportComTest();
    mpOpensslTest = new OpensslTest();
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
    mpCognexDataManSDKTest->TestEnter();
    //mpImportComTest->TestEnter();
    //mpOpensslTest->TestEnter();
}

void TestManager::Exit()
{
    mpDicomFileParserTest->TestExit();
    mpCognexDataManSDKTest->TestExit();
    mpImportComTest->TestExit();
    mpOpensslTest->TestExit();
}
