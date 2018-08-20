#pragma once

class DicomFileParserTest;

class TestManager
{
private:
    DicomFileParserTest *mpDicomFileParserTest;

public:
    ~TestManager();

    static TestManager* Instance();

    void Enter();
    void Exit();

private:
    TestManager();
};
