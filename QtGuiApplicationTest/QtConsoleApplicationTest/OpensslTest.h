#pragma once

class OpensslTest
{
public:
    OpensslTest();
    ~OpensslTest();

    void TestEnter();
    void TestExit();

private:
    int TestASN1();
};
