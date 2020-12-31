#ifndef MISCELLANEOUSQ_QTCPSOCKET_H
#define MISCELLANEOUSQ_QTCPSOCKET_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousQTcpSocket.h"

#include <QAbstractSocket>
#include <QThread>

class QTcpServer;
class QTcpSocket;
class QTcpServerWorker;
class QTcpServerThread;
class QTcpSocketSlotTester;

class MiscellaneousQTcpSocket : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQTcpSocket(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQTcpSocket();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void SlotTcpServerAcceptError(QAbstractSocket::SocketError error);
    void SlotTcpServerNewConnection();
    void SlotTcpSocketClientDisconnected();

    void SlotConnected();
    void SlotDisconnected();
    void SlotError(QAbstractSocket::SocketError socketError);
    void SlotHostFound();
    void SlotProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void SlotStateChanged(QAbstractSocket::SocketState socketState);
    void SlotAboutToClose();
    void SlotBytesWritten(qint64 bytes);
    void SlotChannelBytesWritten(int channel, qint64 bytes);
    void SlotChannelReadyRead(int channel);
    void SlotReadChannelFinished();
    void SlotReadyRead();

    void SlotSocketTimeout();

    void on_btnListen_clicked();
    void on_btnShutdown_clicked();
    void on_btnClearCount_clicked();
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void on_cbAutoConnect_stateChanged(int state);

signals:
    void SignalInitServer(QString listenIP, quint16 listenPort);

private:
    Ui::MiscellaneousQTcpSocket ui;
    QTcpServer                 *mpTcpServer;
    QTcpSocket                 *mpTcpSocket;
    QTcpServerThread           *mpTcpServerThread;
    QTcpServerWorker           *mpTcpServerWorker;
    QThread                    *mpWorkerThread;
    bool                        mAutoConnectFlag;
    int                         mConnectCount;
    int                         mDisconnectCount;
};

class QTcpServerWorker : public QObject
{
    Q_OBJECT

public:
    explicit QTcpServerWorker(QObject *parrent = Q_NULLPTR);
    ~QTcpServerWorker();

private slots:
    void SlotInitServer(QString listenIP, quint16 listenPort);
    void SlotTcpServerWorkerAcceptError(QAbstractSocket::SocketError socketError);
    void SlotTcpServerWorkerNewConnection();

    void SlotTcpServerWorkerClientReadyRead();
    void SlotTcpServerWorkerClientDisconnected();
    void SlotTcpServerWorkerClientError(QAbstractSocket::SocketError socketError);
    void SlotTcpServerWorkerStateChanged(QAbstractSocket::SocketState state);
    void SlotTcpServerWorkerClientDestroyed(QObject *pObj);

private:
    QTcpServer *mpTcpServer;
    int         mConnectCount;
    int         mDisconnectCount;
};

class QTcpServerThread : public QThread
{
    Q_OBJECT

public:
    QTcpServerThread(QObject *parent = Q_NULLPTR);
    ~QTcpServerThread();

    void ClearCount();

protected:
    virtual void run() override;

private slots:
    void SlotTcpServerThreadAcceptError(QAbstractSocket::SocketError socketError);
    void SlotTcpServerThreadNewConnection();

    void SlotTcpServerThreadClientReadyRead();
    void SlotTcpServerThreadClientDisconnected();
    void SlotTcpServerThreadClientError(QAbstractSocket::SocketError socketError);
    void SlotTcpServerThreadStateChanged(QAbstractSocket::SocketState state);
    void SlotTcpServerThreadClientDestroyed(QObject *pObj);

private:
    QTcpServer           *mpTcpServer;
    QTcpSocketSlotTester *mpSlotTester;
    int                   mConnectCount;
    int                   mDisconnectCount;
};

class QTcpSocketSlotTester : public QObject
{
    Q_OBJECT
public:
    QTcpSocketSlotTester(QObject *parent = Q_NULLPTR);
    ~QTcpSocketSlotTester();

    void ClearCount();

private slots:
    void SlotTcpServerThreadAcceptError(QAbstractSocket::SocketError socketError);
    void SlotTcpServerThreadNewConnection();

    void SlotTcpServerThreadClientReadyRead();
    void SlotTcpServerThreadClientDisconnected();
    void SlotTcpServerThreadClientError(QAbstractSocket::SocketError socketError);
    void SlotTcpServerThreadStateChanged(QAbstractSocket::SocketState state);
    void SlotTcpServerThreadClientDestroyed(QObject *pObj);

private:
    int mConnectCount;
    int mDisconnectCount;
};

#endif // MISCELLANEOUSQ_QTCPSOCKET_H
