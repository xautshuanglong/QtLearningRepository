#ifndef MISCELLANEOUSQ_QWEBSOCKET_H
#define MISCELLANEOUSQ_QWEBSOCKET_H

#include <QSslError>
#include <QSslPreSharedKeyAuthenticator>
#include <QWebSocketProtocol>
#include <QNetworkProxy>

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousQWebSocket.h"

class QWebSocket;
class QWebSocketServer;

class MiscellaneousQWebSocket : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQWebSocket(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQWebSocket();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void SlotAcceptError(QAbstractSocket::SocketError socketError);
    void SlotServerSslErrors(const QList<QSslError> &errors);
    void SlotServerError(QWebSocketProtocol::CloseCode closeCode);
    void SlotPeerVerifyError(const QSslError &error);
    void SlotClosed();
    void SlotNewConnection();
    void SlotClientDisconnected();

    void SlotAboutToClose();
    void SlotBinaryFrameReceived(const QByteArray &frame, bool isLastFrame);
    void SlotBinaryMessageReceived(const QByteArray &message);
    void SlotBytesWritten(qint64 bytes);
    void SlotConnected();
    void SlotDisconnected();
    void SlotError(QAbstractSocket::SocketError error);
    void SlotPong(quint64 elapsedTime, const QByteArray &payload);
    void SlotPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void SlotProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void SlotReadChannelFinished();
    void SlotClientSslErrors(const QList<QSslError> &errors);
    void SlotStateChanged(QAbstractSocket::SocketState state);
    void SlotTextFrameReceived(const QString &frame, bool isLastFrame);
    void SlotTextMessageReceived(const QString &message);
    void SlotWebSocketTimeout();

    void on_btnListen_clicked();
    void on_btnShutdown_clicked();
    void on_btnClearCount_clicked();
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void on_cbAutoConnect_stateChanged(int state);

private:
    Ui::MiscellaneousQWebSocket ui;
    QWebSocket                 *mpWebSocket;
    QWebSocketServer           *mpWebSocketServer;
    bool                        mAutoConnectFlag;
    int                         mConnectCount;
    int                         mDisconnectCount;
};

#endif // MISCELLANEOUSQ_QWEBSOCKET_H
