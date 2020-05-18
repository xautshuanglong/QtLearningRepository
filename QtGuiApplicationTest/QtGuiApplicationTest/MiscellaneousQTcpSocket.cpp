#include "MiscellaneousQTcpSocket.h"

MiscellaneousQTcpSocket::MiscellaneousQTcpSocket(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
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
