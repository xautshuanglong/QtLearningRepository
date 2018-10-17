#pragma once

class QTcpSocket;

class ProtocolBufferTest
{
private:
    QTcpSocket       *mpSocketClient;

public:
    ProtocolBufferTest();
    ~ProtocolBufferTest();

    void TestEnter();
    void TestExit();

private:
    void SendMessageTest();
};
