#pragma once

class QTcpSocket;

class ProtocolBufferTest
{
private:
	QTcpSocket       *mpSocketClient1;
	QTcpSocket       *mpSocketClient2;

public:
    ProtocolBufferTest();
    ~ProtocolBufferTest();

    void TestEnter();
    void TestExit();

private:
	bool IsLittleEndian();
    void SendMessageTest();
};
