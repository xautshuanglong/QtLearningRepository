#include "ProtocolBufferTest.h"

#include <QTcpSocket>
#include <QHostInfo>
#include <QThread>

// Protocol Buffer
#include "AddressBook.pb.h"
#include "JCB_Logger/LogUtil.h"

using namespace tutorial;

ProtocolBufferTest::ProtocolBufferTest()
{
    mpSocketClient1 = new QTcpSocket();
    mpSocketClient2 = new QTcpSocket();
}

ProtocolBufferTest::~ProtocolBufferTest()
{
    delete mpSocketClient1;
    mpSocketClient1 = nullptr;

    delete mpSocketClient2;
    mpSocketClient2 = nullptr;
}

void ProtocolBufferTest::TestEnter()
{
    this->SendMessageTest();
}

void ProtocolBufferTest::TestExit()
{
    mpSocketClient1->disconnectFromHost();
    mpSocketClient1->waitForDisconnected();
    LogUtil::Debug(CODE_LOCATION, "mpSocketClient1 has disconnected ...");

    mpSocketClient2->disconnectFromHost();
    mpSocketClient2->waitForDisconnected();
    LogUtil::Debug(CODE_LOCATION, "mpSocketClient2 has disconnected ...");
}

bool ProtocolBufferTest::IsLittleEndian()
{
    bool retValue = true;
    int endianTest = 0x12345678;
    char *pTest = (char*)&endianTest;
    if (*pTest == 0x12)
    {
        retValue = false;
    }
    return retValue;
}

void ProtocolBufferTest::SendMessageTest()
{
    LogUtil::Debug(CODE_LOCATION, "------- SendMessageTest -------");

    QString strLocalHostName = QHostInfo::localHostName();
    LogUtil::Debug(CODE_LOCATION, "Local Host Name: %s", strLocalHostName.toStdString().c_str());

    QString serverIP = "localhost";
    int serverPort = 8000;

    mpSocketClient1->connectToHost(serverIP, serverPort);
    LogUtil::Debug(CODE_LOCATION, "mpSocketClient1 connectToHost %s %d", serverIP.toStdString().c_str(), serverPort);
    bool connectFlag1 = mpSocketClient1->waitForConnected(5000);
    LogUtil::Debug(CODE_LOCATION, "mpSocketClient1 connectToHost %s !", connectFlag1 ? "successfully" : "failed");

    mpSocketClient2->connectToHost(serverIP, serverPort);
    LogUtil::Debug(CODE_LOCATION, "mpSocketClient2 connectToHost %s %d", serverIP.toStdString().c_str(), serverPort);
    bool connectFlag2 = mpSocketClient2->waitForConnected(5000);
    LogUtil::Debug(CODE_LOCATION, "mpSocketClient2 connectToHost %s !", connectFlag2 ? "successfully" : "failed");

    Person_PhoneNumber phoneNumber;
    phoneNumber.set_number("18792441258");
    phoneNumber.set_type(Person_PhoneType_MOBILE);

    Person_PhoneNumber homeNumber;
    homeNumber.set_number("0558-4327497");
    homeNumber.set_type(Person_PhoneType_HOME);

    Person person;
    person.set_name("Shuanglong");
    person.set_id(666);
    person.set_email("xjshuanglong@126.com");
    Person_PhoneNumber *pPhoneAdd = person.add_phones();
    pPhoneAdd->CopyFrom(phoneNumber);
    pPhoneAdd = person.add_phones();
    pPhoneAdd->CopyFrom(homeNumber);
    google::protobuf::Timestamp* pCurTimestamp = new google::protobuf::Timestamp();
    pCurTimestamp->set_seconds(::time(NULL));
    pCurTimestamp->set_nanos(0);
    person.set_allocated_last_updated(pCurTimestamp); // 内部自动释放 Timestamp

    LogUtil::Debug(CODE_LOCATION, "Timestamp.seconds: %lld", pCurTimestamp->seconds());

    int msgBodyLen = person.ByteSize();
    int msgLen = msgBodyLen + 4;
    char *pTempMsgBuffer = new char[msgLen];
    memcpy(pTempMsgBuffer, &msgLen, 4);
    person.SerializeToArray(pTempMsgBuffer +4, msgBodyLen);

    if (connectFlag1)
    {
        //mpSocketClient->setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
        int count = 0;
        while (count < 5)
        {
            mpSocketClient1->write(pTempMsgBuffer, msgLen);
            //mpSocketClient1->waitForBytesWritten();

            //if (connectFlag2)
            //{
            //    mpSocketClient2->write("SocketClient2 test");
            //    mpSocketClient2->waitForBytesWritten();
            //}
            //mpSocketClient->flush();
            LogUtil::Debug(CODE_LOCATION, "after write %d !", count);
            QThread::msleep(500);
            ++count;
        }
        mpSocketClient1->close();
    }

    delete[]pTempMsgBuffer;
}
