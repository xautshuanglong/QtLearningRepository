#pragma once

class DicomFileParserTest;
class CognexDataManSDKTest;
class ImportComTest;

class TestManager
{
private:
    DicomFileParserTest    *mpDicomFileParserTest;
    CognexDataManSDKTest   *mpCognexDataManSDKTest;
    ImportComTest          *mpImportComTest;

public:
    ~TestManager();

    static TestManager* Instance();

    void Enter();
    void Exit();

private:
    TestManager();
};
