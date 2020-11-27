#include "MiscellaneousWinSocket.h"

#include <QWindow>

#pragma warning (disable: 6387)

MiscellaneousWinSocket::MiscellaneousWinSocket(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousWinSocket::~MiscellaneousWinSocket()
{
}

QString MiscellaneousWinSocket::GetTitle()
{
    return QObject::tr("Win Socket");
}

QString MiscellaneousWinSocket::GetTitleTooltip()
{
    return QObject::tr("How to use windows API network communication.");
}

MiscellaneousTestGroup MiscellaneousWinSocket::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinSocket::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Thread_WinSocket;
}

void MiscellaneousWinSocket::on_btnListen_clicked()
{
    int i = 0;
}

void MiscellaneousWinSocket::on_btnStop_clicked()
{
    int i = 0;
}
