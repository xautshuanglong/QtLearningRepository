#include "MiscellaneousWinSocket.h"

#define FD_SETSIZE 64 // to increase max socket size in fdset
#include <WinSock2.h>
#include <limits.h>
#include <thread>

#include <QWindow>

#include "LogUtil.h"

#pragma warning (disable: 6387)

#define WM_USER_ASYNCSELECT_MSG     WM_USER + 1

MiscellaneousWinSocket::MiscellaneousWinSocket(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
    , mServerMode(EnumServerSocketMode::CStyleSelect)
    , mThreadListen(nullptr)
    , mMsgWinHandle(NULL)
{
    ui.setupUi(this);

    // 套接字环境初始化
    // version 2.2 = a.b --> a | (b << 8)
    // The high-order byte specifies the minor version number;
    // the low-order byte specifies the major version number.
    WSADATA wsaData;
    int retVal = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (retVal != 0)
    {
        LogUtil::Error(CODE_LOCATION, "WSAStartup start failed! ErrorCode: %d", retVal);
    }
    else
    {
        LogUtil::Info(CODE_LOCATION, "WSAStartup start successfully: CurreentVersion=%hu.%hu HighestVersion=%hu.%u MaxSockets=%hu",
                      LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion), LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion),
                      wsaData.iMaxSockets);
    }

    // 监听线程
    mThreadListen = new std::thread(std::bind(&MiscellaneousWinSocket::ThreadFunction, this));
    // 并发测试线程
    //unsigned int hardwareConcurrency = std::thread::hardware_concurrency();
    //for (int i = 0; i < hardwareConcurrency - 2; ++i)
    //{
    //    mListThreads.push_back(new std::thread(std::bind(&MiscellaneousWinSocket::ThreadFunction, this)));
    //}
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

    ::WSACleanup();
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
    WNDCLASS window;
    HWND windowHandle = NULL;

    // set the window properties.
    memset(&window, 0, sizeof(WNDCLASS));
    window.lpszClassName = "Accept Window";
    window.hInstance = NULL;
    //window.lpfnWndProc = std::bind(&MiscellaneousWinSocket::WindowProcess, this);
    window.hCursor = NULL;
    window.hIcon = NULL;
    window.lpszMenuName = NULL;
    window.hbrBackground = NULL;
    window.style = 0;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;

    // register the window class.
    if (!RegisterClass(&window)) {
        printf("Registerclass failed %d\n", GetLastError());
    }

    windowHandle = CreateWindow("Accept Window",
                                "Accept Window",
                                WS_OVERLAPPEDWINDOW,    //WS_MINIMIZE,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                (HWND)NULL,
                                (HMENU)NULL,
                                (HINSTANCE)NULL,
                                (LPVOID)NULL);
    // check if a window was created.
    if (windowHandle == NULL)
    {
        printf("CreateWindow failed %d\n", GetLastError());
    }
}

void MiscellaneousWinSocket::Win32AsynchronizeStop()
{
    int i = 0;
}

void MiscellaneousWinSocket::Win32PollStart()
{
    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll
    // demo : Windows-classic-samples/Samples/Win7Samples/netds/winsock/wsapoll/poll.cpp

}

void MiscellaneousWinSocket::Win32PollStop()
{
    ;
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
        if (countLow > 1000)
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

LRESULT MiscellaneousWinSocket::WindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT res = 0;

    // check what the message type is.
    switch (message) {

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        // this is the message that that we gave in WSAAsyncSelect which
        // winsock will send us back for notifying us of socket events.
    case WM_USER_ASYNCSELECT_MSG:
        //ProcessAsyncSelectMessage(wParam, lParam);
        break;
    default:
        res = DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return res;
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
