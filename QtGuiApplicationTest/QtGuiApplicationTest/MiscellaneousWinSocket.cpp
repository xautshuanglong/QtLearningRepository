#include "MiscellaneousWinSocket.h"

#include <thread>

#include <QWindow>

#pragma warning (disable: 6387)

MiscellaneousWinSocket::MiscellaneousWinSocket(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
    , mServerMode(EnumServerSocketMode::CStyleSelect)
    , mpThreadListen(nullptr)
{
    ui.setupUi(this);
    mpThreadListen = new std::thread(std::bind(&MiscellaneousWinSocket::ThreadFunction, this));
}

MiscellaneousWinSocket::~MiscellaneousWinSocket()
{
    if (mpThreadListen != nullptr)
    {
        if (mpThreadListen->joinable())
        {
            mpThreadListen->join();
        }
        delete mpThreadListen;
    }
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

void MiscellaneousWinSocket::CStyleSelectStart()
{
    int i = 0;
}

void MiscellaneousWinSocket::CStyleSelectStop()
{
    int i = 0;
}

void MiscellaneousWinSocket::Win32SelectStart()
{
    int i = 0;
}

void MiscellaneousWinSocket::Win32SelectStop()
{
    int i = 0;
}

void MiscellaneousWinSocket::Win32AsynchronizeStart()
{
    int i = 0;
}

void MiscellaneousWinSocket::Win32AsynchronizeStop()
{
    int i = 0;
}

void MiscellaneousWinSocket::CompletionPortStart()
{
    uint hardwareConcurrentcy = std::thread::hardware_concurrency();
    int i = 0;
}

void MiscellaneousWinSocket::CompletionPortStop()
{
    int i = 0;
}

void MiscellaneousWinSocket::ThreadFunction()
{

    int i = 0;
}

void MiscellaneousWinSocket::on_cbServerMode_currentIndexChanged(int index)
{
    mServerMode = EnumServerSocketMode(index);
}

void MiscellaneousWinSocket::on_btnListen_clicked()
{
    switch (mServerMode)
    {
    case EnumServerSocketMode::CStyleSelect:
        this->CStyleSelectStart();
        break;
    case EnumServerSocketMode::Win32Select:
        this->Win32SelectStart();
        break;
    case EnumServerSocketMode::Win32Asynchronize:
        this->Win32AsynchronizeStart();
        break;
    case EnumServerSocketMode::CompletionPort:
        this->CompletionPortStart();
        break;
    default:
        break;
    }
}

void MiscellaneousWinSocket::on_btnStop_clicked()
{
    switch (mServerMode)
    {
    case EnumServerSocketMode::CStyleSelect:
        this->CStyleSelectStop();
        break;
    case EnumServerSocketMode::Win32Select:
        this->Win32SelectStop();
        break;
    case EnumServerSocketMode::Win32Asynchronize:
        this->Win32AsynchronizeStop();
        break;
    case EnumServerSocketMode::CompletionPort:
        this->CompletionPortStop();
        break;
    default:
        break;
    }
}
