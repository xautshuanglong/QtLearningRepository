#include "MiscellaneousQWebSocket.h"

#include <QWebSocket>
#include <QWebSocketServer>
#include <QSslKey>
#include <QMetaEnum>
#include <QTimer>

#include <LogUtil.h>

#define WEBSOCKET_PORT  1259
#define CERT_FILE_NAME  "server.crt"
#define KEY_FILE_NAME   "server.key"
#define KEY_FILE_PHRASE "testing"
#define SSL_SECRET_PATH "/tlskey/"

MiscellaneousQWebSocket::MiscellaneousQWebSocket(QWidget *parent)
    : MiscellaneousBase(parent)
    , m_pWebSocket(Q_NULLPTR)
    , m_pWebSocketServer(Q_NULLPTR)
    , m_autoConnectFlag(false)
{
    ui.setupUi(this);

    ui.leServerUrl->setText("wss://127.0.0.1:1259");

    // 初始化服务端
    QSslConfiguration sslConfiguration;
    QString exePath = QCoreApplication::applicationDirPath();
    QFile certFile(exePath + QString(SSL_SECRET_PATH) + QString(CERT_FILE_NAME));
    QFile keyFile(exePath + QString(SSL_SECRET_PATH) + QString(KEY_FILE_NAME));
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, KEY_FILE_PHRASE);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
    m_pWebSocketServer = new QWebSocketServer(QString("QWebSocket server testing"), QWebSocketServer::SecureMode, this);
    m_pWebSocketServer->setMaxPendingConnections(5000);
    m_pWebSocketServer->setSslConfiguration(sslConfiguration);
    connect(m_pWebSocketServer, SIGNAL(acceptError(QAbstractSocket::SocketError)),  this, SLOT(SlotAcceptError(QAbstractSocket::SocketError)));
    connect(m_pWebSocketServer, SIGNAL(sslErrors(const QList<QSslError> &)),        this, SLOT(SlotServerSslErrors(const QList<QSslError> &)));
    connect(m_pWebSocketServer, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(SlotServerError(QWebSocketProtocol::CloseCode)));
    connect(m_pWebSocketServer, SIGNAL(peerVerifyError(const QSslError &)),         this, SLOT(SlotPeerVerifyError(const QSslError &)));
    connect(m_pWebSocketServer, SIGNAL(closed()),                                   this, SLOT(SlotClosed()));
    connect(m_pWebSocketServer, SIGNAL(newConnection()),                            this, SLOT(SlotNewConnection()));

    // 初始化客户端
    m_pWebSocket = new QWebSocket(QString("QWebSocket client testing"), QWebSocketProtocol::VersionLatest, this);
    QSslConfiguration sslConfig = m_pWebSocket->sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_pWebSocket->setProxy(QNetworkProxy::NoProxy);
    m_pWebSocket->setSslConfiguration(sslConfig);
    this->connect(m_pWebSocket, SIGNAL(aboutToClose()),                                                       this, SLOT(SlotAboutToClose()));
    this->connect(m_pWebSocket, SIGNAL(binaryFrameReceived(const QByteArray &, bool)),                        this, SLOT(SlotBinaryFrameReceived(const QByteArray &, bool)));
    this->connect(m_pWebSocket, SIGNAL(binaryMessageReceived(const QByteArray &)),                            this, SLOT(SlotBinaryMessageReceived(const QByteArray &)));
    this->connect(m_pWebSocket, SIGNAL(bytesWritten(qint64)),                                                 this, SLOT(SlotBytesWritten(qint64)));
    this->connect(m_pWebSocket, SIGNAL(connected()),                                                          this, SLOT(SlotConnected()));
    this->connect(m_pWebSocket, SIGNAL(disconnected()),                                                       this, SLOT(SlotDisconnected()));
    this->connect(m_pWebSocket, SIGNAL(error(QAbstractSocket::SocketError)),                                  this, SLOT(SlotError(QAbstractSocket::SocketError)));
    this->connect(m_pWebSocket, SIGNAL(pong(quint64, const QByteArray &)),                                    this, SLOT(SlotPong(quint64, const QByteArray &)));
    this->connect(m_pWebSocket, SIGNAL(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)),  this, SLOT(SlotPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)));
    this->connect(m_pWebSocket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)), this, SLOT(SlotProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
    this->connect(m_pWebSocket, SIGNAL(readChannelFinished()),                                                this, SLOT(SlotReadChannelFinished()));
    this->connect(m_pWebSocket, SIGNAL(sslErrors(const QList<QSslError> &)),                                  this, SLOT(SlotClientSslErrors(const QList<QSslError> &)));
    this->connect(m_pWebSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),                           this, SLOT(SlotStateChanged(QAbstractSocket::SocketState)));
    this->connect(m_pWebSocket, SIGNAL(textFrameReceived(const QString &, bool)),                             this, SLOT(SlotTextFrameReceived(const QString &, bool)));
    this->connect(m_pWebSocket, SIGNAL(textMessageReceived(const QString &)),                                 this, SLOT(SlotTextMessageReceived(const QString &)));

    // 自动连接测试定时器
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

void MiscellaneousQWebSocket::SlotAcceptError(QAbstractSocket::SocketError socketError)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotServerSslErrors(const QList<QSslError> &errors)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotServerError(QWebSocketProtocol::CloseCode closeCode)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotPeerVerifyError(const QSslError &error)
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotClosed()
{
    int i = 0;
}

void MiscellaneousQWebSocket::SlotNewConnection()
{
    static int connectCount = 0;
    while (m_pWebSocketServer->hasPendingConnections())
    {
        QWebSocket *pTempSocket = m_pWebSocketServer->nextPendingConnection();
        if (pTempSocket)
        {
            ++connectCount;
            LogUtil::Debug(CODE_LOCATION, "QWebSocketClient connect count: %d", connectCount);
            this->connect(pTempSocket, SIGNAL(disconnected()), SLOT(SlotClientDisconnected()));
        }
    }
}

void MiscellaneousQWebSocket::SlotClientDisconnected()
{
    static int disconnectCount = 0;
    ++disconnectCount;
    LogUtil::Debug(CODE_LOCATION, "QWebSocketClient disconnect count: %d", disconnectCount);
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

void MiscellaneousQWebSocket::SlotClientSslErrors(const QList<QSslError> &errors)
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

void MiscellaneousQWebSocket::on_btnListen_clicked()
{
    bool listenFlag = m_pWebSocketServer->listen(QHostAddress::Any, WEBSOCKET_PORT);
    if (listenFlag)
    {
        int i = 0;
    }
}

void MiscellaneousQWebSocket::on_btnShutdown_clicked()
{
    m_pWebSocketServer->close();
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
