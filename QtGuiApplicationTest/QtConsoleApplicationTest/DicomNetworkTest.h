#pragma once

class DicomNetworkTest
{
public:
    DicomNetworkTest();
    ~DicomNetworkTest();

    void TestEnter();
    void TestExit();

private:
    void EchoSCU();
};
