#include "TestManager.h"

#include "DicomFileParserTest.h"

TestManager::TestManager()
{
    mpDicomFileParserTest = new DicomFileParserTest();
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
}

void TestManager::Exit()
{
    mpDicomFileParserTest->TestExit();
}
