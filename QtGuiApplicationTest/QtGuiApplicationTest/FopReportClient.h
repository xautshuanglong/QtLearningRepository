#pragma once

#include <QObject>
#include <QTcpSocket>

class FopReportClient : public QObject
{
    Q_OBJECT
public:
    FopReportClient();
    ~FopReportClient();

    void SendData(const char* data, qint64 dataSize);

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
};
