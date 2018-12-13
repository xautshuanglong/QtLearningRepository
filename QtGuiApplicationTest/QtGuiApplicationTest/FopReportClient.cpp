#include "FopReportClient.h"

#include <QDataStream>
#include <QMetaEnum>
#include <LogUtil.h>

// Protocol Buffer
#include "MessageInfo.pb.h"
using namespace com::genomics::protobuf;

int FopReportClient::gMsgID = 1;

FopReportClient::FopReportClient()
    : mIsConnected(false)
{
    // 选取关心的信号：连接建立、连接断开、错误处理
    this->connect(&mTcpSocket, SIGNAL(connected()), SLOT(SlotConnected()));
    this->connect(&mTcpSocket, SIGNAL(disconnected()), SLOT(SlotDisconnected()));
    this->connect(&mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(SlotSocketError(QAbstractSocket::SocketError)));
    //this->connect(&mTcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(SlotStateChanged(QAbstractSocket::SocketState)));
    this->connect(&mTcpSocket, SIGNAL(readyRead()), SLOT(SlotReadyRead()));
    this->connect(&mTcpSocket, SIGNAL(bytesWritten(qint64)), SLOT(SlotBytesWritten(qint64)));
    //this->connect(&mTcpSocket, SIGNAL(channelReadyRead(int)), SLOT(SlotChannelReadyRead(int)));
    //this->connect(&mTcpSocket, SIGNAL(channelBytesWritten(int, qint64)), SLOT(SlotChannelBytesWritten(int, qint64)));
    //this->connect(&mTcpSocket, SIGNAL(readChannelFinished()), SLOT(SlotReadChannelFinished()));
}

FopReportClient::~FopReportClient()
{
    if (mTcpSocket.isOpen())
    {
        mTcpSocket.close();
    }
}

void FopReportClient::SendData(QByteArray data)
{
    if (!mIsConnected)
    {
        mTcpSocket.connectToHost("127.0.0.1", 8000);
        mTcpSocket.waitForConnected(5000);
    }
    mTcpSocket.write(data);
}

void FopReportClient::SavePDF(const QString& cmd, const QString& xmlFile, const QString& xslFile, const QString& outFile)
{
    QByteArray cmdBuffer = this->PackMessageCommand(cmd, xmlFile, xslFile, outFile);
    this->SendData(cmdBuffer);
}

QByteArray FopReportClient::PackMessageCommand(const QString& cmd, const QString& xmlFile, const QString& xslFile, const QString& outFile)
{
    MessageHeader *pMsgHeader = new MessageHeader();
    pMsgHeader->set_msgid(gMsgID++);
    pMsgHeader->set_version(1);
    pMsgHeader->set_msgtype(MsgType::MsgTypeCommand);
    MessageCommand *pMsgCommand = new MessageCommand();
    pMsgCommand->set_cmd(cmd.toStdString().c_str());
    pMsgCommand->set_xmlfilename(xmlFile.toStdString().c_str());
    pMsgCommand->set_xslfilename(xslFile.toStdString().c_str());
    pMsgCommand->set_outfilename(outFile.toStdString().c_str());

    MessageBody *pMsgBody = new MessageBody();
    pMsgBody->set_allocated_msgcommand(pMsgCommand);
    MessageInfo msgInfo;
    msgInfo.set_allocated_msgheader(pMsgHeader);
    msgInfo.set_allocated_msgbody(pMsgBody);

    int msgBodyLen = msgInfo.ByteSize();
    int msgLen = msgBodyLen + 4;

    QByteArray retByteArray;
    QDataStream msgStream(&retByteArray, QIODevice::WriteOnly);
    msgStream.setByteOrder(QDataStream::BigEndian);
    char *pTempMsgBuffer = new char[msgBodyLen];
    msgInfo.SerializeToArray(pTempMsgBuffer, msgBodyLen);
    msgStream.writeBytes(pTempMsgBuffer, msgBodyLen); // 注：QDataStream 内部机制，前4字符为data的长度，后面紧跟data内容。
    msgStream.device()->seek(0);
    msgStream << msgLen;
    delete[]pTempMsgBuffer;

    return retByteArray;
}

void FopReportClient::MessageResponseHandler(MessageHeader *pMsgHeader, MessageBody *pMsgBody)
{
    MessageResponse msgResponse = pMsgBody->msgresponse();

    int responseID = msgResponse.responseid();
    const std::string description = msgResponse.description();
    ResponseType responseType = msgResponse.responsetype();

    LogUtil::Debug(CODE_LOCATION, "MsgResponseID[%d] MsgResponseType[%s] MsgDescription[%s]",
                   responseID,
                   ::ResponseType_Name(responseType).c_str(),
                   description.c_str());

    if (responseType == ResTypeSaveSuccess)
    {
        emit SignalSaveCompletely();
    }
}

void FopReportClient::SlotConnected()
{
    mIsConnected = true;
    LogUtil::Debug(CODE_LOCATION, "QTcpSocket connected");
}

void FopReportClient::SlotDisconnected()
{
    mIsConnected = false;
    LogUtil::Debug(CODE_LOCATION, "QTcpSocket disconnected");
}

void FopReportClient::SlotSocketError(QAbstractSocket::SocketError socketError)
{
    QMetaEnum socketErrorEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    LogUtil::Error(CODE_LOCATION, "SocketError: %s", socketErrorEnum.valueToKey(socketError));
}

void FopReportClient::SlotStateChanged(QAbstractSocket::SocketState socketState)
{
    QMetaEnum socketErrorEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    LogUtil::Error(CODE_LOCATION, "SocketState: %s", socketErrorEnum.valueToKey(socketState));
}

void FopReportClient::SlotReadyRead()
{
    char readData[512] = { 0 };
    int readByteCount = mTcpSocket.read(readData, 512);

    MessageInfo responseMsg;
    responseMsg.ParseFromArray(readData + 4, readByteCount - 4);

    MessageHeader msgHeader = responseMsg.msgheader();
    MessageBody msgBody = responseMsg.msgbody();
    MsgType msgType = msgHeader.msgtype();
    int msgID = msgHeader.msgid();
    int msgVersion = msgHeader.version();
    //LogUtil::Debug(CODE_LOCATION, "MsgVersion[%d] MsgType[%s] MsgID[%d]",
    //               msgVersion, ::MsgType_Name(msgType).c_str(), msgID);

    // VLD 内存泄漏检测
    LogUtil::Debug(CODE_LOCATION, "MsgVersion[%d] MsgType[%d] MsgID[%d]",
                   msgVersion, msgType, msgID);

    if (msgType != MsgTypeUnknow)
    {
        switch (msgType)
        {
        case com::genomics::protobuf::MsgTypeRequest:
            break;
        case com::genomics::protobuf::MsgTypeResponse:
            this->MessageResponseHandler(&msgHeader, &msgBody);
        break;
        case com::genomics::protobuf::MsgTypePush:
            break;
        case com::genomics::protobuf::MsgTypePull:
            break;
        case com::genomics::protobuf::MsgTypeCommand:
            break;
        default:
            break;
        }
    }
}

void FopReportClient::SlotBytesWritten(qint64 bytesCount)
{
    LogUtil::Debug(CODE_LOCATION, "QTcpSocket: %d bytes written", bytesCount);
}

void FopReportClient::SlotChannelBytesWritten(int channel, qint64 bytesCount)
{
    LogUtil::Debug(CODE_LOCATION, "QTcpSocket: channel %d write %d bytes", channel, bytesCount);
}

void FopReportClient::SlotChannelReadyRead(int channel)
{
    LogUtil::Debug(CODE_LOCATION, "QTcpSocket: channel %d ready read", channel);
}

void FopReportClient::SlotReadChannelFinished()
{
    LogUtil::Debug(CODE_LOCATION, "QTcpSocket: read channel finished");
}
