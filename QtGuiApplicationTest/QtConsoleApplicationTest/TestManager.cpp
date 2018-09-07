#include "TestManager.h"

#include "DicomFileParserTest.h"
#include "DicomNetworkTest.h"
#include "CognexDataManSDKTest.h"
#include "ImportComTest.h"
#include "OpensslTest.h"

TestManager::TestManager()
{
    mpDicomFileParserTest = new DicomFileParserTest();
    mpDicomNetworkTest = new DicomNetworkTest();
    mpCognexDataManSDKTest = new CognexDataManSDKTest();
    mpImportComTest = new ImportComTest();
    mpOpensslTest = new OpensslTest();
}

TestManager::~TestManager()
{
    delete mpDicomFileParserTest;
    delete mpDicomNetworkTest;
    delete mpCognexDataManSDKTest;
    delete mpImportComTest;
    delete mpOpensslTest;
}

TestManager* TestManager::Instance()
{
    static TestManager instance;
    return &instance;
}

void TestManager::Enter()
{
    //mpDicomFileParserTest->TestEnter();
    mpDicomNetworkTest->TestEnter();
    //mpCognexDataManSDKTest->TestEnter();
    //mpImportComTest->TestEnter();
    //mpOpensslTest->TestEnter();
}

void TestManager::Exit()
{
    mpDicomFileParserTest->TestExit();
    mpDicomNetworkTest->TestExit();
    mpCognexDataManSDKTest->TestExit();
    mpImportComTest->TestExit();
    mpOpensslTest->TestExit();
}
