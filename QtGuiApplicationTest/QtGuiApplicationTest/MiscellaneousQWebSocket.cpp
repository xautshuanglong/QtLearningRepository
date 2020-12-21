#include "MiscellaneousQWebSocket.h"

#include <QWebSocket>
#include <QMetaEnum>
#include <QTimer>

#include <LogUtil.h>

MiscellaneousQWebSocket::MiscellaneousQWebSocket(QWidget *parent)
    : MiscellaneousBase(parent)
    , m_pWebSocket(Q_NULLPTR)
    , m_autoConnectFlag(false)
{
    ui.setupUi(this);

    ui.leServerUrl->setText("wss://127.0.0.1:7893");

    m_pWebSocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
    m_pWebSocket->setProxy(QNetworkProxy::NoProxy);

    QSslConfiguration sslConfig = m_pWebSocket->sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_pWebSocket->setSslConfiguration(sslConfig);

    this->connect(m_pWebSocket, SIGNAL(aboutToClose()), this, SLOT(SlotAboutToClose()));
    this->connect(m_pWebSocket, SIGNAL(binaryFrameReceived(const QByteArray &, bool)), this, SLOT(SlotBinaryFrameReceived(const QByteArray &, bool)));
    this->connect(m_pWebSocket, SIGNAL(binaryMessageReceived(const QByteArray &)), this, SLOT(SlotBinaryMessageReceived(const QByteArray &)));
    this->connect(m_pWebSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(SlotBytesWritten(qint64)));
    this->connect(m_pWebSocket, SIGNAL(connected()), this, SLOT(SlotConnected()));
    this->connect(m_pWebSocket, SIGNAL(disconnected()), this, SLOT(SlotDisconnected()));
    this->connect(m_pWebSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
    this->connect(m_pWebSocket, SIGNAL(pong(quint64, const QByteArray &)), this, SLOT(SlotPong(quint64, const QByteArray &)));
    this->connect(m_pWebSocket, SIGNAL(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)), this, SLOT(SlotPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)));
    this->connect(m_pWebSocket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)), this, SLOT(SlotProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
    this->connect(m_pWebSocket, SIGNAL(readChannelFinished()), this, SLOT(SlotReadChannelFinished()));
    this->connect(m_pWebSocket, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(SlotSslErrors(const QList<QSslError> &)));
    this->connect(m_pWebSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotStateChanged(QAbstractSocket::SocketState)));
    this->connect(m_pWebSocket, SIGNAL(textFrameReceived(const QString &, bool)), this, SLOT(SlotTextFrameReceived(const QString &, bool)));
    this->connect(m_pWebSocket, SIGNAL(textMessageReceived(const QString &)), this, SLOT(SlotTextMessageReceived(const QString &)));

    QTimer *pSocketTimer = new QTimer(this);
    this->connect(pSocketTimer, SIGNAL(timeout()), this, SLOT(SlotWebSocketTimeout()));
    pSocketTimer->start(10);
}

MiscellaneousQWebSocket::~MiscellaneousQWebSocket()
{
}

QString MiscellaneousQWebSocket::GetTitle()
{
    return QObject::tr("QWebSocket Client");
}

QString MiscellaneousQWebSocket::GetTitleTooltip()
{
    return QObject::tr("For QWebSocketServer pressure testing");
}

MiscellaneousTestGroup MiscellaneousQWebSocket::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQWebSocket::GetItemID()
{
    return MiscellaneousTestItem::QT_WebSocket_Client;
}

void MiscellaneousQWebSocket::SlotAboutToClose()
{
    LogUtil::Warn(CODE_LOCATION, "QWebSocket is about to close ...");
}

void MiscellaneousQWebSocket::SlotBinaryFrameReceived(const QByteArray &frame, bool isLastFrame)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotBinaryMessageReceived(const QByteArray &message)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotBytesWritten(qint64 bytes)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotConnected()
{
    LogUtil::Info(CODE_LOCATION, "QWebSocket connected --------------");
}

void MiscellaneousQWebSocket::SlotDisconnected()
{
    LogUtil::Info(CODE_LOCATION, "QWebSocket disconnected --------------");
}

void MiscellaneousQWebSocket::SlotError(QAbstractSocket::SocketError error)
{
    QMetaEnum socketErrorEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    QString socketErrorStr = socketErrorEnum.valueToKeys(error);
    LogUtil::Error(CODE_LOCATION, "QWebSocket Error: %s", socketErrorStr.toUtf8().data());
}

void MiscellaneousQWebSocket::SlotPong(quint64 elapsedTime, const QByteArray &payload)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotReadChannelFinished()
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotSslErrors(const QList<QSslError> &errors)
{
    for (QSslError sslError : errors)
    {
        LogUtil::Error(CODE_LOCATION, "QWebSocket SSL Error[%d]: %s", sslError.error(), sslError.errorString().toUtf8().data());
    }
}

void MiscellaneousQWebSocket::SlotStateChanged(QAbstractSocket::SocketState state)
{
    QMetaEnum socketStateEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString socketStateStr = socketStateEnum.valueToKeys(state);
    LogUtil::Error(CODE_LOCATION, "QWebSocket State changed: %s", socketStateStr.toUtf8().data());
}

void MiscellaneousQWebSocket::SlotTextFrameReceived(const QString &frame, bool isLastFrame)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotTextMessageReceived(const QString &message)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotWebSocketTimeout()
{
    if (m_autoConnectFlag)
    {
        this->on_btnConnect_clicked();
    }
}

void MiscellaneousQWebSocket::on_btnConnect_clicked()
{
    QString urlString = ui.leServerUrl->text();
    if (urlString.isEmpty())
    {
        LogUtil::Error(CODE_LOCATION, "The server url is empty ...");
        return;
    }

    QUrl serverUrl(urlString, QUrl::StrictMode);
    if (serverUrl.isValid())
    {
        QString testStr = serverUrl.errorString();
        if (m_pWebSocket->isValid())
        {
            m_pWebSocket->close();
        }
        m_pWebSocket->open(serverUrl);
    }
    else
    {
        LogUtil::Error(CODE_LOCATION, "The server url is invalid: %s", serverUrl.errorString().toUtf8().data());
    }
}

void MiscellaneousQWebSocket::on_btnDisconnect_clicked()
{
    if (m_pWebSocket->isValid())
    {
        m_pWebSocket->close();
    }
}

void MiscellaneousQWebSocket::on_cbAutoConnect_stateChanged(int state)
{
    m_autoConnectFlag = state == Qt::Checked;
}
