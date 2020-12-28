#include "MiscellaneousQTcpSocket.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QMetaEnum>
#include <QTimer>

#include <LogUtil.h>

#define TCP_SERVER_PORT  1259

MiscellaneousQTcpSocket::MiscellaneousQTcpSocket(QWidget *parent)
    : MiscellaneousBase(parent)
    , mpTcpServer(new QTcpServer(this))
    , mpTcpSocket(new QTcpSocket(this))
    , mAutoConnectFlag(false)
    , mConnectCount(0)
    , mDisconnectCount(0)
{
    ui.setupUi(this);

    ui.leServerIP->setText("127.0.0.1");
    ui.leServerPort->setText("1259");
    ui.leListenIP->setText("0.0.0.0");
    ui.leListenPort->setText("1259");

    // 初始化服务端 QTcpServer
    this->connect(mpTcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerAcceptError(QAbstractSocket::SocketError)));
    this->connect(mpTcpServer, SIGNAL(newConnection()), this, SLOT(SlotTcpServerNewConnection()));
    
    // 初始化客户端 QTcpSocket
    this->connect(mpTcpSocket, SIGNAL(connected()), this, SLOT(SlotConnected()));
    this->connect(mpTcpSocket, SIGNAL(disconnected()), this, SLOT(SlotDisconnected()));
    this->connect(mpTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
    this->connect(mpTcpSocket, SIGNAL(hostFound()), this, SLOT(SlotHostFound()));
    this->connect(mpTcpSocket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)), this, SLOT(SlotProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
    this->connect(mpTcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotStateChanged(QAbstractSocket::SocketState)));
    this->connect(mpTcpSocket, SIGNAL(aboutToClose()), this, SLOT(SlotAboutToClose()));
    this->connect(mpTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(SlotBytesWritten(qint64)));
    this->connect(mpTcpSocket, SIGNAL(channelBytesWritten(int, qint64)), this, SLOT(SlotChannelBytesWritten(int, qint64)));
    this->connect(mpTcpSocket, SIGNAL(channelReadyRead(int)), this, SLOT(SlotChannelReadyRead(int)));
    this->connect(mpTcpSocket, SIGNAL(readChannelFinished()), this, SLOT(SlotReadChannelFinished()));
    this->connect(mpTcpSocket, SIGNAL(readyRead()), this, SLOT(SlotReadyRead()));

    // 定时连接断开
    QTimer *pSocketTimer = new QTimer(this);
    this->connect(pSocketTimer, SIGNAL(timeout()), this, SLOT(SlotSocketTimeout()));
    pSocketTimer->start(10);
}

MiscellaneousQTcpSocket::~MiscellaneousQTcpSocket()
{
}

QString MiscellaneousQTcpSocket::GetTitle()
{
    return QObject::tr("QTcpSocket Server & Client");
}

QString MiscellaneousQTcpSocket::GetTitleTooltip()
{
    return QObject::tr("For QTcpServer pressure testing");
}

MiscellaneousTestGroup MiscellaneousQTcpSocket::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQTcpSocket::GetItemID()
{
    return MiscellaneousTestItem::QT_TcpSocket_Client;
}

void MiscellaneousQTcpSocket::SlotTcpServerAcceptError(QAbstractSocket::SocketError error)
{
    QMetaEnum socketErrorEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    QString socketErrorStr = socketErrorEnum.valueToKeys(error);
    LogUtil::Error(CODE_LOCATION, "QTcpServer Accept Error: %s", socketErrorStr.toUtf8().data());
}

void MiscellaneousQTcpSocket::SlotTcpServerNewConnection()
{
    ++mConnectCount;

    while (mpTcpServer->hasPendingConnections())
    {
        QTcpSocket *pTempSocket = mpTcpServer->nextPendingConnection();
        if (pTempSocket)
        {
            QString peerAddr = pTempSocket->peerAddress().toString();
            quint16 peerPort = pTempSocket->peerPort();
            LogUtil::Info(CODE_LOCATION, "QTcpServer client connect count: %d 0x%08X", mConnectCount, pTempSocket);
            //this->connect(pTempSocket, SIGNAL(readyRead()), this, SLOT(SlotTcpClientReadyRead()));
            this->connect(pTempSocket, SIGNAL(disconnected()), this, SLOT(SlotTcpSocketClientDisconnected()));
            //this->connect(pTempSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotTcpError(QAbstractSocket::SocketError)));
            //this->connect(pTempSocket, SIGNAL(disconnected()), pTempSocket, SLOT(deleteLater()), Qt::QueuedConnection);
            //this->connect(pTempSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotTcpStateChanged(QAbstractSocket::SocketState)));
            //this->connect(pTempSocket, SIGNAL(destroyed(QObject *)), this, SLOT(SlotTcpClientDestroyed(QObject *)));
        }
    }
}

void MiscellaneousQTcpSocket::SlotTcpSocketClientDisconnected()
{
    ++mDisconnectCount;

    QTcpSocket *pTcpSocket = reinterpret_cast<QTcpSocket *>(sender());
    pTcpSocket->deleteLater();
    LogUtil::Info(CODE_LOCATION, "QTcpServer client disconnect count: %d 0x%08X", mDisconnectCount, pTcpSocket);
}

void MiscellaneousQTcpSocket::SlotConnected()
{
    LogUtil::Info(CODE_LOCATION, "QTcpSocket connected --------------");
    mpTcpSocket->write("hello world ...");
}

void MiscellaneousQTcpSocket::SlotDisconnected()
{
    LogUtil::Info(CODE_LOCATION, "QTcpSocket disconnected --------------");
}

void MiscellaneousQTcpSocket::SlotError(QAbstractSocket::SocketError socketError)
{
    QMetaEnum socketErrorEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    QString socketErrorStr = socketErrorEnum.valueToKeys(socketError);
    LogUtil::Error(CODE_LOCATION, "QTcpSocket Error: %s", socketErrorStr.toUtf8().data());
}

void MiscellaneousQTcpSocket::SlotHostFound()
{
    LogUtil::Info(CODE_LOCATION, "QTcpSocket host found --------------");
}

void MiscellaneousQTcpSocket::SlotProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
}

void MiscellaneousQTcpSocket::SlotStateChanged(QAbstractSocket::SocketState socketState)
{
    QMetaEnum socketStateEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString socketStateStr = socketStateEnum.valueToKeys(socketState);
    LogUtil::Error(CODE_LOCATION, "QTcpSocket State changed: %s", socketStateStr.toUtf8().data());
}

void MiscellaneousQTcpSocket::SlotAboutToClose()
{
    LogUtil::Warn(CODE_LOCATION, "QTcpSocket is about to close ...");
}

void MiscellaneousQTcpSocket::SlotBytesWritten(qint64 bytes)
{
}

void MiscellaneousQTcpSocket::SlotChannelBytesWritten(int channel, qint64 bytes)
{
}

void MiscellaneousQTcpSocket::SlotChannelReadyRead(int channel)
{
}

void MiscellaneousQTcpSocket::SlotReadChannelFinished()
{
}

void MiscellaneousQTcpSocket::SlotReadyRead()
{
}

void MiscellaneousQTcpSocket::SlotSocketTimeout()
{
    if (mAutoConnectFlag)
    {
        this->on_btnConnect_clicked();
    }
}

void MiscellaneousQTcpSocket::on_btnListen_clicked()
{
    QString serverIpStr = ui.leServerIP->text();
    QString serverPortStr = ui.leServerPort->text();
    quint16 serverPort = serverPortStr.isEmpty() ? TCP_SERVER_PORT : serverPortStr.toInt();
    QHostAddress hostAddress = serverIpStr.isEmpty() ? QHostAddress::Any : QHostAddress(serverIpStr);

    bool listenFlag = mpTcpServer->listen(hostAddress, serverPort);
    if (listenFlag)
    {
        int i = 0;
    }
}

void MiscellaneousQTcpSocket::on_btnShutdown_clicked()
{
    mpTcpServer->close();
}

void MiscellaneousQTcpSocket::on_btnClearCount_clicked()
{
    mConnectCount = 0;
    mDisconnectCount = 0;
}

void MiscellaneousQTcpSocket::on_btnConnect_clicked()
{
    QString serverIP = ui.leServerIP->text();
    QString serverPortStr = ui.leServerPort->text();
    if (serverIP.isEmpty() || serverPortStr.isEmpty())
    {
        LogUtil::Error(CODE_LOCATION, "The server ip or port is empty ...");
        return;
    }

    bool resultFlag = false;
    quint16 serverPort = serverPortStr.toInt(&resultFlag);
    if (resultFlag)
    {
        if (mpTcpSocket->isValid())
        {
            mpTcpSocket->disconnectFromHost();
            mpTcpSocket->waitForDisconnected();
        }
        mpTcpSocket->connectToHost(serverIP, serverPort);
        mpTcpSocket->waitForConnected();
    }
    else
    {
        LogUtil::Error(CODE_LOCATION, "Can not convert server port from string to int");
    }
}

void MiscellaneousQTcpSocket::on_btnDisconnect_clicked()
{
    if (mpTcpSocket->isValid())
    {
        mpTcpSocket->disconnectFromHost();
    }
}

void MiscellaneousQTcpSocket::on_cbAutoConnect_stateChanged(int state)
{
    mAutoConnectFlag = state == Qt::Checked;
}
