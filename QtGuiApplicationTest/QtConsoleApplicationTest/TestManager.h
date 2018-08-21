#pragma once

class DicomFileParserTest;
class CognexDataManSDKTest;

class TestManager
{
private:
    DicomFileParserTest    *mpDicomFileParserTest;
    CognexDataManSDKTest   *mpCognexDataManSDKTest;

public:
    ~TestManager();

    static TestManager* Instance();

    void Enter();
    void Exit();

private:
    TestManager();
};
