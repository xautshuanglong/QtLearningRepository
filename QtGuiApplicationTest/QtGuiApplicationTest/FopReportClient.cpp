#include "FopReportClient.h"

#include <QMetaEnum>
#include <LogUtil.h>

FopReportClient::FopReportClient()
    : mIsConnected(false)
{
    // 选取关心的信号：连接建立、连接断开、错误处理
    this->connect(&mTcpSocket, SIGNAL(connected()), SLOT(SlotConnected()));
    this->connect(&mTcpSocket, SIGNAL(disconnected()), SLOT(SlotDisconnected()));
    this->connect(&mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(SlotSocketError(QAbstractSocket::SocketError)));
    //this->connect(&mTcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(SlotStateChanged(QAbstractSocket::SocketState)));
    this->connect(&mTcpSocket, SIGNAL(readyRead()), SLOT(SlotReadyRead()));
    this->connect(&mTcpSocket, SIGNAL(channelReadyRead(int)), SLOT(SlotChannelReadyRead(int)));
    this->connect(&mTcpSocket, SIGNAL(bytesWritten(qint64)), SLOT(SlotBytesWritten(qint64)));
    this->connect(&mTcpSocket, SIGNAL(channelBytesWritten(int, qint64)), SLOT(SlotChannelBytesWritten(int, qint64)));
    //this->connect(&mTcpSocket, SIGNAL(readChannelFinished()), SLOT(SlotReadChannelFinished()));
}

FopReportClient::~FopReportClient()
{
}

void FopReportClient::SendData(const char* data, qint64 dataSize)
{
    if (!mIsConnected)
    {
        mTcpSocket.connectToHost("127.0.0.1", 8000);
        mTcpSocket.waitForConnected(5000);
    }
    mTcpSocket.write(data, dataSize);
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
    LogUtil::Debug(CODE_LOCATION, "QTcpSocket: ready Read");
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
