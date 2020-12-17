#include "MiscellaneousWinSocket.h"

#include <thread>
#include <limits.h> 
#include <windows.h>

#include <QWindow>

#pragma warning (disable: 6387)

MiscellaneousWinSocket::MiscellaneousWinSocket(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
    , mServerMode(EnumServerSocketMode::CStyleSelect)
    , mThreadListen(nullptr)
{
    ui.setupUi(this);

    // 套接字环境初始化

    // 监听线程
    mThreadListen = new std::thread(std::bind(&MiscellaneousWinSocket::ThreadFunction, this));
    // 并发测试线程
    unsigned int hardwareConcurrency = std::thread::hardware_concurrency();
    for (int i = 0; i < hardwareConcurrency - 2; ++i)
    {
        mListThreads.push_back(new std::thread(std::bind(&MiscellaneousWinSocket::ThreadFunction, this)));
    }
}

MiscellaneousWinSocket::~MiscellaneousWinSocket()
{
    if (mThreadListen != nullptr)
    {
        if (mThreadListen->joinable())
        {
            mThreadListen->join();
        }
        delete mThreadListen;
    }

    for (std::thread *pThread : mListThreads)
    {
        if (pThread->joinable())
        {
            pThread->join();
        }
        delete pThread;
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

void MiscellaneousWinSocket::Win32AsynchronizeStart()
{
    //WSAAsyncSelect()
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
    static ULONGLONG countHigh = 0;
    static ULONGLONG countLow = 0;
    countLow = 0;
    while (true)
    {
        ++countLow;
        if (countLow > 100000000000)
        {
            break;
        }
        //if (countLow == ULLONG_MAX)
        //{
        //    countLow = 0;
        //    ++countHigh;
        //    if (countHigh == ULLONG_MAX)
        //    {
        //        break;
        //    }
        //}
    }
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
