#pragma once

class DicomFileParserTest
{
public:
    DicomFileParserTest();
    ~DicomFileParserTest();

    void TestEnter();
    void TestExit();

private:
    void ParseDicomFile();
};
