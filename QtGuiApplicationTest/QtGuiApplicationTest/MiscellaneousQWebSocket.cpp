#include "MiscellaneousQWebSocket.h"

MiscellaneousQWebSocket::MiscellaneousQWebSocket(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
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
    return MiscellaneousTestItem::QT_WEBSOCKET_CLIENT;
}

void MiscellaneousQWebSocket::on_btnConnect_clicked()
{
    int i = 0;
}

void MiscellaneousQWebSocket::on_btnDisconnect_clicked()
{
    int i = 0;
}
