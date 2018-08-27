#pragma once

namespace CognexDataManSDK
{
    class DataManManager;
}

class CognexDataManSDKTest
{
public:
    CognexDataManSDKTest();
    ~CognexDataManSDKTest();

    void TestEnter();
    void TestExit();

private:
    void DataManManagerTest();

private:
    CognexDataManSDK::DataManManager  *mpDataManManager;
};
