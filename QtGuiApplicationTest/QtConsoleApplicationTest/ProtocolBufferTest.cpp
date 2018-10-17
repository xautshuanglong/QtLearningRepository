#include "ProtocolBufferTest.h"

#include <QTcpSocket>
#include <QHostInfo>
#include <QThread>

#include <LogUtil.h>

ProtocolBufferTest::ProtocolBufferTest()
{
    mpSocketClient = new QTcpSocket();
}

ProtocolBufferTest::~ProtocolBufferTest()
{
    delete mpSocketClient;
    mpSocketClient = nullptr;
}

void ProtocolBufferTest::TestEnter()
{
    this->SendMessageTest();
}

void ProtocolBufferTest::TestExit()
{
    mpSocketClient->disconnectFromHost();
    mpSocketClient->waitForDisconnected();
    LogUtil::Debug(CODE_LOCATION, "mpSocketClient has disconnected ...");
}

void ProtocolBufferTest::SendMessageTest()
{
    LogUtil::Debug(CODE_LOCATION, "------- SendMessageTest -------");

    QString strLocalHostName = QHostInfo::localHostName();
    LogUtil::Debug(CODE_LOCATION, "Local Host Name: %s", strLocalHostName.toStdString().c_str());

    QString serverIP = "localhost";
    int serverPort = 8000;
    mpSocketClient->connectToHost(serverIP, serverPort);
    LogUtil::Debug(CODE_LOCATION, "connectToHost %s %d", serverIP.toStdString().c_str(), serverPort);
    bool connectFlag = mpSocketClient->waitForConnected(5000);
    LogUtil::Debug(CODE_LOCATION, "connectToHost %s !", connectFlag ? "successfully" : "failed");

    if (connectFlag)
    {
        //mpSocketClient->setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
        char tempMsg[100] = { 0 };
        int count = 0;
        while (count < 10)
        {
            sprintf(tempMsg, "%s%d", "Hello-", count);
            mpSocketClient->write(tempMsg);
            mpSocketClient->waitForBytesWritten();
            //mpSocketClient->flush();
            LogUtil::Debug(CODE_LOCATION, "after write %d !", count);
            QThread::msleep(500);
            ++count;
        }
        mpSocketClient->close();
    }
}
