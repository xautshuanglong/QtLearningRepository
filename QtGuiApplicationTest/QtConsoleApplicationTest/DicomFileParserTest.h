#pragma once

#include <string>

class DcmObject;

class DicomFileParserTest
{
public:
    DicomFileParserTest();
    ~DicomFileParserTest();

    void TestEnter();
    void TestExit();

private:
    void ParseDicomFile();
    void GetDcmElementStringValue(DcmObject *pInDcmObj, std::string& outString);
};
