#include "MiscellaneousQTcpSocket.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QMetaEnum>
#include <QTimer>

#include "JCB_Logger/LogUtil.h"

#define TCP_SERVER_PORT         1259
#define TCP_SERVER_THREAD_PORT  1258
#define TCP_SERVER_WORKER_PORT  1257

#define SIGNAL_SLOT_DIRECT_CONNECTION 0
#define MOVE_QTHREAD_TO_ITSELF        0
#define CONNECT_TO_SLOT_TESTER        1

MiscellaneousQTcpSocket::MiscellaneousQTcpSocket(QWidget *parent)
    : MiscellaneousBase(parent)
    , mpTcpServer(new QTcpServer(this))
    , mpTcpSocket(new QTcpSocket(this))
#if MOVE_QTHREAD_TO_ITSELF
    , mpTcpServerThread(new QTcpServerThread()) // 要移动到线程内部的对象不能有父对象
#else
    , mpTcpServerThread(new QTcpServerThread(this))
#endif
    , mpTcpServerWorker(new QTcpServerWorker())
    , mpWorkerThread(new QThread(this))
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

    // QTcpServerWorker moveToThread 测试
    mpTcpServerWorker->moveToThread(mpWorkerThread);
    this->connect(this, SIGNAL(SignalInitServer(QString, quint16)), mpTcpServerWorker, SLOT(SlotInitServer(QString, quint16)));

    // 定时连接断开
    QTimer *pSocketTimer = new QTimer(this);
    this->connect(pSocketTimer, SIGNAL(timeout()), this, SLOT(SlotSocketTimeout()));
    pSocketTimer->start(4);
}

MiscellaneousQTcpSocket::~MiscellaneousQTcpSocket()
{
    mpTcpServer->close();

    mpTcpServerThread->quit();
    mpTcpServerThread->wait();
#if MOVE_QTHREAD_TO_ITSELF
    delete mpTcpServerThread; // 移动到线程内部，没有父对象，需手动删除。
    mpTcpServerThread = Q_NULLPTR;
#endif

    mpTcpServerWorker->deleteLater();

    mpWorkerThread->quit();
    mpWorkerThread->wait();
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
            //this->connect(pTempSocket, SIGNAL(disconnected()), pTempSocket, SLOT(deleteLater()));
            this->connect(pTempSocket, SIGNAL(disconnected()), this, SLOT(SlotTcpSocketClientDisconnected()));
            //this->connect(pTempSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotTcpError(QAbstractSocket::SocketError)));
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
    LogUtil::Info(CODE_LOCATION, "QTcpSocket State changed: %s", socketStateStr.toUtf8().data());
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
    QString listenIpStr = ui.leListenIP->text();
    QString listenPortStr = ui.leListenPort->text();
    quint16 listenPort = listenPortStr.isEmpty() ? TCP_SERVER_PORT : listenPortStr.toInt();
    QHostAddress hostAddress = listenIpStr.isEmpty() ? QHostAddress::Any : QHostAddress(listenIpStr);

    bool listenFlag = mpTcpServer->listen(hostAddress, listenPort);
    if (listenFlag)
    {
        int i = 0;
    }

    if (!mpTcpServerThread->isRunning())
    {
        mpTcpServerThread->setObjectName("Q_TcpServer_Thread");
        mpTcpServerThread->start();
    }
    if (!mpWorkerThread->isRunning())
    {
        mpWorkerThread->setObjectName("Q_TcpServer_Worker_Thread");
        mpWorkerThread->start();
        emit SignalInitServer("0.0.0.0", TCP_SERVER_WORKER_PORT);
    }
}

void MiscellaneousQTcpSocket::on_btnShutdown_clicked()
{
    mpTcpServer->close();
    
    mpTcpServerThread->quit();
    mpTcpServerThread->wait();

    mpWorkerThread->quit();
    mpWorkerThread->wait();

    int i = 0;
}

void MiscellaneousQTcpSocket::on_btnClearCount_clicked()
{
    mConnectCount = 0;
    mDisconnectCount = 0;
    mpTcpServerThread->ClearCount();
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

/*========================================== QTcpServerWorker =========================================*/

QTcpServerWorker::QTcpServerWorker(QObject *parrent /* = Q_NULLPTR*/)
    : QObject(parrent)
    , mpTcpServer(new QTcpServer(this))
    , mConnectCount(0)
    , mDisconnectCount(0)
{
    this->connect(mpTcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerWorkerAcceptError(QAbstractSocket::SocketError)));
    this->connect(mpTcpServer, SIGNAL(newConnection()), this, SLOT(SlotTcpServerWorkerNewConnection()));
}

QTcpServerWorker::~QTcpServerWorker()
{
}

void QTcpServerWorker::SlotInitServer(QString listenIP, quint16 listenPort)
{
    bool listenFlag = mpTcpServer->listen(QHostAddress(listenIP), listenPort);
    if (listenFlag)
    {
        int i = 0;
    }
}

void QTcpServerWorker::SlotTcpServerWorkerAcceptError(QAbstractSocket::SocketError socketError)
{
    int i = 0;
}

void QTcpServerWorker::SlotTcpServerWorkerNewConnection()
{
    QTcpServer *pTcpServer = reinterpret_cast<QTcpServer *>(sender());
    while (pTcpServer->hasPendingConnections())
    {
        QTcpSocket *pTempSocket = pTcpServer->nextPendingConnection();
        if (pTempSocket)
        {
            QString peerAddr = pTempSocket->peerAddress().toString();
            quint16 peerPort = pTempSocket->peerPort();
            LogUtil::Info(CODE_LOCATION, "QTcpServerWorker client connect count: %d 0x%08X", ++mConnectCount, pTempSocket);
            //this->connect(pTempSocket, SIGNAL(readyRead()), this, SLOT(SlotTcpServerWorkerClientReadyRead()));
            this->connect(pTempSocket, SIGNAL(disconnected()), this, SLOT(SlotTcpServerWorkerClientDisconnected()));
            this->connect(pTempSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerWorkerClientError(QAbstractSocket::SocketError)));
            this->connect(pTempSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotTcpServerWorkerStateChanged(QAbstractSocket::SocketState)));
            this->connect(pTempSocket, SIGNAL(destroyed(QObject *)), this, SLOT(SlotTcpServerWorkerClientDestroyed(QObject *)));
        }
    }
}

void QTcpServerWorker::SlotTcpServerWorkerClientReadyRead()
{
}

void QTcpServerWorker::SlotTcpServerWorkerClientDisconnected()
{
    QTcpSocket *pTempSocket = reinterpret_cast<QTcpSocket *>(sender());
    pTempSocket->deleteLater();
    LogUtil::Info(CODE_LOCATION, "QTcpServerWorker client disconnect count: %d 0x%08X", ++mDisconnectCount, pTempSocket);
}

void QTcpServerWorker::SlotTcpServerWorkerClientError(QAbstractSocket::SocketError socketError)
{
}

void QTcpServerWorker::SlotTcpServerWorkerStateChanged(QAbstractSocket::SocketState state)
{
}

void QTcpServerWorker::SlotTcpServerWorkerClientDestroyed(QObject *pObj)
{
}

/*========================================== QTcpServerThread =========================================*/

QTcpServerThread::QTcpServerThread(QObject *parent /* = Q_NULLPTR */)
    : QThread(parent)
    , mpTcpServer(Q_NULLPTR)
    , mpSlotTester(Q_NULLPTR)
    , mConnectCount(0)
    , mDisconnectCount(0)
{
#if MOVE_QTHREAD_TO_ITSELF
    this->moveToThread(this); // 继承 QThread 且确保自身槽函数运行在子线程中
#endif
}

QTcpServerThread::~QTcpServerThread()
{
    if (mpTcpServer)
    {
        delete mpTcpServer;
        mpTcpServer = Q_NULLPTR;
    }
    if (mpSlotTester)
    {
        delete mpSlotTester;
        mpSlotTester = Q_NULLPTR;
    }
}

void QTcpServerThread::ClearCount()
{
    mConnectCount = 0;
    mDisconnectCount = 0;
    if (mpSlotTester)
    {
        mpSlotTester->ClearCount();
    }
}

void QTcpServerThread::run()
{
    if (mpSlotTester == Q_NULLPTR)
    {
        mpSlotTester = new QTcpSocketSlotTester();
    }
    if (mpTcpServer == Q_NULLPTR)
    {
        mpTcpServer = new QTcpServer();
    }
    mpTcpServer->listen(QHostAddress::Any, TCP_SERVER_THREAD_PORT);
#if SIGNAL_SLOT_DIRECT_CONNECTION
    this->connect(mpTcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerThreadAcceptError(QAbstractSocket::SocketError)), Qt::DirectConnection);
    this->connect(mpTcpServer, SIGNAL(newConnection()), this, SLOT(SlotTcpServerThreadNewConnection()), Qt::DirectConnection);
#elif CONNECT_TO_SLOT_TESTER
    this->connect(mpTcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), mpSlotTester, SLOT(SlotTcpServerThreadAcceptError(QAbstractSocket::SocketError)));
    this->connect(mpTcpServer, SIGNAL(newConnection()), mpSlotTester, SLOT(SlotTcpServerThreadNewConnection()));
#else
    this->connect(mpTcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerThreadAcceptError(QAbstractSocket::SocketError)));
    this->connect(mpTcpServer, SIGNAL(newConnection()), this, SLOT(SlotTcpServerThreadNewConnection()));
#endif
    this->exec();

    mpTcpServer->close();
    delete mpTcpServer;
    mpTcpServer = Q_NULLPTR;
    delete mpSlotTester;
    mpSlotTester = Q_NULLPTR;
}

void QTcpServerThread::SlotTcpServerThreadAcceptError(QAbstractSocket::SocketError socketError)
{
    ;
}

void QTcpServerThread::SlotTcpServerThreadNewConnection()
{
    //QTcpServer *pTcpServer = reinterpret_cast<QTcpServer*>(sender()); // 直连信号槽，无法获取发送者。
    while (mpTcpServer->hasPendingConnections())
    {
        QTcpSocket *pTempSocket = mpTcpServer->nextPendingConnection();
        if (pTempSocket)
        {
            QString peerAddr = pTempSocket->peerAddress().toString();
            quint16 peerPort = pTempSocket->peerPort();
            LogUtil::Info(CODE_LOCATION, "QTcpServerThread client connect count: %d 0x%08X", ++mConnectCount, pTempSocket);
#if SIGNAL_SLOT_DIRECT_CONNECTION
            //this->connect(pTempSocket, SIGNAL(readyRead()), this, SLOT(SlotTcpServerThreadClientReadyRead()));
            this->connect(pTempSocket, SIGNAL(disconnected()), pTempSocket, SLOT(deleteLater()));
            this->connect(pTempSocket, SIGNAL(disconnected()), this, SLOT(SlotTcpServerThreadClientDisconnected()), Qt::DirectConnection);
            this->connect(pTempSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerThreadClientError(QAbstractSocket::SocketError)), Qt::DirectConnection);
            this->connect(pTempSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotTcpServerThreadStateChanged(QAbstractSocket::SocketState)), Qt::DirectConnection);
            this->connect(pTempSocket, SIGNAL(destroyed(QObject*)), this, SLOT(SlotTcpServerThreadClientDestroyed(QObject*)), Qt::DirectConnection);
#else
            this->connect(pTempSocket, SIGNAL(readyRead()), this, SLOT(SlotTcpServerThreadClientReadyRead()));
            this->connect(pTempSocket, SIGNAL(disconnected()), this, SLOT(SlotTcpServerThreadClientDisconnected()));
            this->connect(pTempSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerThreadClientError(QAbstractSocket::SocketError)));
            this->connect(pTempSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotTcpServerThreadStateChanged(QAbstractSocket::SocketState)));
            this->connect(pTempSocket, SIGNAL(destroyed(QObject *)), this, SLOT(SlotTcpServerThreadClientDestroyed(QObject *)));
#endif
        }
    }
}

void QTcpServerThread::SlotTcpServerThreadClientReadyRead()
{
}

void QTcpServerThread::SlotTcpServerThreadClientDisconnected()
{
    QTcpSocket *pTempSocket = qobject_cast<QTcpSocket*>(this->sender());
    if (pTempSocket != Q_NULLPTR)
    {
        pTempSocket->deleteLater();
    }
    LogUtil::Info(CODE_LOCATION, "QTcpServerThread client disconnect count: %d 0x%08X", ++mDisconnectCount, pTempSocket);
}

void QTcpServerThread::SlotTcpServerThreadClientError(QAbstractSocket::SocketError socketError)
{
}

void QTcpServerThread::SlotTcpServerThreadStateChanged(QAbstractSocket::SocketState state)
{
}

void QTcpServerThread::SlotTcpServerThreadClientDestroyed(QObject *pObj)
{
}

QTcpSocketSlotTester::QTcpSocketSlotTester(QObject *parent /* = Q_NULLPTR */)
    : QObject(parent)
    , mConnectCount(0)
    , mDisconnectCount(0)
{
}

QTcpSocketSlotTester::~QTcpSocketSlotTester()
{
}

void QTcpSocketSlotTester::ClearCount()
{
    mDisconnectCount = 0;
}

void QTcpSocketSlotTester::SlotTcpServerThreadAcceptError(QAbstractSocket::SocketError socketError)
{
    ;
}

void QTcpSocketSlotTester::SlotTcpServerThreadNewConnection()
{
    QTcpServer *pTcpServer = qobject_cast<QTcpServer*>(sender());
    while (pTcpServer->hasPendingConnections())
    {
        QTcpSocket *pTempSocket = pTcpServer->nextPendingConnection();
        if (pTempSocket)
        {
            QString peerAddr = pTempSocket->peerAddress().toString();
            quint16 peerPort = pTempSocket->peerPort();
            LogUtil::Info(CODE_LOCATION, "QTcpServerThread client connect count: %d 0x%08X", ++mConnectCount, pTempSocket);
#if SIGNAL_SLOT_DIRECT_CONNECTION
            //this->connect(pTempSocket, SIGNAL(readyRead()), this, SLOT(SlotTcpServerThreadClientReadyRead()), Qt::DirectConnection);
            this->connect(pTempSocket, SIGNAL(disconnected()), pTempSocket, SLOT(deleteLater()));
            this->connect(pTempSocket, SIGNAL(disconnected()), this, SLOT(SlotTcpServerThreadClientDisconnected()), Qt::DirectConnection);
            this->connect(pTempSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerThreadClientError(QAbstractSocket::SocketError)), Qt::DirectConnection);
            this->connect(pTempSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotTcpServerThreadStateChanged(QAbstractSocket::SocketState)), Qt::DirectConnection);
            this->connect(pTempSocket, SIGNAL(destroyed(QObject *)), this, SLOT(SlotTcpServerThreadClientDestroyed(QObject *)), Qt::DirectConnection);
#else
            this->connect(pTempSocket, SIGNAL(readyRead()), this, SLOT(SlotTcpServerThreadClientReadyRead()));
            this->connect(pTempSocket, SIGNAL(disconnected()), this, SLOT(SlotTcpServerThreadClientDisconnected()));
            this->connect(pTempSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotTcpServerThreadClientError(QAbstractSocket::SocketError)));
            this->connect(pTempSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SlotTcpServerThreadStateChanged(QAbstractSocket::SocketState)));
            this->connect(pTempSocket, SIGNAL(destroyed(QObject *)), this, SLOT(SlotTcpServerThreadClientDestroyed(QObject *)));
#endif
        }
    }
}

void QTcpSocketSlotTester::SlotTcpServerThreadClientReadyRead()
{
}

void QTcpSocketSlotTester::SlotTcpServerThreadClientDisconnected()
{
    QTcpSocket *pTempSocket = qobject_cast<QTcpSocket *>(this->sender());
    if (pTempSocket)
    {
        pTempSocket->deleteLater();
    }
    LogUtil::Info(CODE_LOCATION, "QTcpSocketSlotTester client disconnect count: %d 0x%08X", ++mDisconnectCount, pTempSocket);
}

void QTcpSocketSlotTester::SlotTcpServerThreadClientError(QAbstractSocket::SocketError socketError)
{
}

void QTcpSocketSlotTester::SlotTcpServerThreadStateChanged(QAbstractSocket::SocketState state)
{
}

void QTcpSocketSlotTester::SlotTcpServerThreadClientDestroyed(QObject *pObj)
{
}
