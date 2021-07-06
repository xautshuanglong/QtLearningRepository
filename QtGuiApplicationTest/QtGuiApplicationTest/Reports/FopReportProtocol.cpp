#include "FopReportProtocol.h"

#include <QDataStream>

// Protocol Buffer
#include "MessageInfo.pb.h"
using namespace com::genomics::protobuf;

#include <LogUtil.h>

int FopReportProtocol::gMsgID = 0;

FopReportProtocol::FopReportProtocol()
{
}

FopReportProtocol::~FopReportProtocol()
{
}

FopReportProtocol* FopReportProtocol::GetInstance()
{
    static FopReportProtocol instance;
    return &instance;
}

void FopReportProtocol::ReadMessage(QTcpSocket& socket)
{
    char data[512] = { 0 };
    int readCount = socket.read(data, 512);
    this->ParseMessage(data, readCount);
}

void FopReportProtocol::ParseMessage(const char *pData, int dataLen)
{
    MessageInfo responseMsg;
    responseMsg.ParseFromArray(pData+4, dataLen-4);

    int msgID = 0;
    int msgVersion = 0;
    MsgType msgType = MsgTypeUnknow;

    MessageHeader msgHeader = responseMsg.msgheader();
    msgID = msgHeader.msgid();
    msgVersion = msgHeader.version();
    msgType = msgHeader.msgtype();
    LogUtil::Debug(CODE_LOCATION, "MsgVersion[%d] MsgType[%s] MsgID[%d]",
                   msgVersion, ::MsgType_Name(msgType).c_str(), msgID);

    MessageBody msgBody = responseMsg.msgbody();

    if (msgType != MsgTypeUnknow)
    {
        switch (msgType)
        {
        case com::genomics::protobuf::MsgTypeRequest:
            break;
        case com::genomics::protobuf::MsgTypeResponse:
        {
            MessageResponse msgResponse = msgBody.msgresponse();
            int responseID = msgResponse.responseid();
            ResponseType responseType = msgResponse.responsetype();
            const std::string description = msgResponse.description();
            LogUtil::Debug(CODE_LOCATION, "MsgResponseID[%d] ResponseType[%s] MsgDescription[%s]",
                           responseID, ::ResponseType_Name(responseType), description.c_str());
        }
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

QByteArray FopReportProtocol::PackMessageCommand(const QString& cmd, const QString& xmlFile, const QString& xslFile, const QString& outFile)
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
