#include "MiscellaneousQTcpSocket.h"

#include <QTcpSocket>
#include <QMetaEnum>
#include <QTimer>

#include <LogUtil.h>

MiscellaneousQTcpSocket::MiscellaneousQTcpSocket(QWidget *parent)
    : MiscellaneousBase(parent)
    , m_pTcpSocket(new QTcpSocket(this))
    , m_autoConnectFlag(false)
{
    ui.setupUi(this);

    ui.leServerIP->setText("127.0.0.1");
    ui.leServerPort->setText("7893");

    this->connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(SlotConnected()));
    this->connect(m_pTcpSocket, SIGNAL(disconnected()), this, SLOT(SlotDisconnected()));
    this->connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
    this->connect(m_pTcpSocket, SIGNAL(hostFound()), this, SLOT(SlotHostFound()));
    this->connect(m_pTcpSocket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)), this, SLOT(SlotProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
    this->connect(m_pTcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotStateChanged(QAbstractSocket::SocketState)));
    this->connect(m_pTcpSocket, SIGNAL(aboutToClose()), this, SLOT(SlotAboutToClose()));
    this->connect(m_pTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(SlotBytesWritten(qint64)));
    this->connect(m_pTcpSocket, SIGNAL(channelBytesWritten(int, qint64)), this, SLOT(SlotChannelBytesWritten(int, qint64)));
    this->connect(m_pTcpSocket, SIGNAL(channelReadyRead(int)), this, SLOT(SlotChannelReadyRead(int)));
    this->connect(m_pTcpSocket, SIGNAL(readChannelFinished()), this, SLOT(SlotReadChannelFinished()));
    this->connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(SlotReadyRead()));

    QTimer *pSocketTimer = new QTimer(this);
    this->connect(pSocketTimer, SIGNAL(timeout()), this, SLOT(SlotSocketTimeout()));
    pSocketTimer->start(100);
}

MiscellaneousQTcpSocket::~MiscellaneousQTcpSocket()
{
}

QString MiscellaneousQTcpSocket::GetTitle()
{
    return QObject::tr("QTcpSocket Client");
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
    return MiscellaneousTestItem::QT_TCPSOCKET_CLIENT;
}

void MiscellaneousQTcpSocket::SlotConnected()
{
    LogUtil::Info(CODE_LOCATION, "QTcpSocket connected --------------");
    m_pTcpSocket->write("hello world ...");
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
    if (m_autoConnectFlag)
    {
        this->on_btnConnect_clicked();
    }
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
        if (m_pTcpSocket->isValid())
        {
            m_pTcpSocket->disconnectFromHost();
        }
        m_pTcpSocket->connectToHost(serverIP, serverPort);
    }
    else
    {
        LogUtil::Error(CODE_LOCATION, "Can not convert server port from string to int");
    }
}

void MiscellaneousQTcpSocket::on_btnDisconnect_clicked()
{
    if (m_pTcpSocket->isValid())
    {
        m_pTcpSocket->disconnectFromHost();
    }
}

void MiscellaneousQTcpSocket::on_cbAutoConnect_stateChanged(int state)
{
    m_autoConnectFlag = state == Qt::Checked;
}
