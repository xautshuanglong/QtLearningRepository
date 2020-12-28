#ifndef MISCELLANEOUSQ_QTCPSOCKET_H
#define MISCELLANEOUSQ_QTCPSOCKET_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousQTcpSocket.h"

#include <QAbstractSocket>
#include <QThread>

class QTcpServer;
class QTcpSocket;

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

private:
    Ui::MiscellaneousQTcpSocket ui;
    QTcpServer                 *mpTcpServer;
    QTcpSocket                 *mpTcpSocket;
    bool                        mAutoConnectFlag;
    int                         mConnectCount;
    int                         mDisconnectCount;
};

class QTcpServerThread : public QThread
{
    ;
};

#endif // MISCELLANEOUSQ_QTCPSOCKET_H
