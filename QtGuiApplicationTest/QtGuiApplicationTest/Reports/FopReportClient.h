#pragma once

#include <QObject>
#include <QTcpSocket>

namespace com
{
    namespace genomics
    {
        namespace protobuf
        {
            class MessageHeader;
            class MessageBody;
        }
    }
}

class FopReportClient : public QObject
{
    Q_OBJECT
public:
    FopReportClient();
    ~FopReportClient();

    void SavePDF(const QString& cmd, const QString& xmlFile, const QString& xslFile, const QString& outFile);

private:
    void SendData(QByteArray data);
    QByteArray PackMessageCommand(const QString& cmd, const QString& xmlFile, const QString& xslFile, const QString& outFile);
    void MessageResponseHandler(com::genomics::protobuf::MessageHeader *pMsgHeader,
                                com::genomics::protobuf::MessageBody *pMsgBody);

signals:
    void SignalSaveCompletely();

private slots:
    void SlotConnected();
    void SlotDisconnected();
    void SlotSocketError(QAbstractSocket::SocketError socketError);
    void SlotStateChanged(QAbstractSocket::SocketState socketState);
    void SlotReadyRead();
    void SlotBytesWritten(qint64 bytesCount);
    void SlotChannelBytesWritten(int channel, qint64 bytesCount);
    void SlotChannelReadyRead(int channel);
    void SlotReadChannelFinished();

private:
    QTcpSocket  mTcpSocket;
    bool        mIsConnected;
    static int  gMsgID;
};
