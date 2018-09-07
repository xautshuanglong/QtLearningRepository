#pragma once

class DicomFileParserTest;
class DicomNetworkTest;
class CognexDataManSDKTest;
class ImportComTest;
class OpensslTest;

class TestManager
{
private:
    DicomFileParserTest    *mpDicomFileParserTest;
    DicomNetworkTest       *mpDicomNetworkTest;
    CognexDataManSDKTest   *mpCognexDataManSDKTest;
    ImportComTest          *mpImportComTest;
    OpensslTest            *mpOpensslTest;

public:
    ~TestManager();

    static TestManager* Instance();

    void Enter();
    void Exit();

private:
    TestManager();
};
