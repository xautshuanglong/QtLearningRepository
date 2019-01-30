#include "DebugInfoNetworkWidget.h"
#include "ui_DebugInfoNetworkWidget.h"

// QT Headers
#include <QMetaEnum>

// Windows Headers
#include <WinSock2.h>

// Self Headers
#include <LogUtil.h>
#include "DebugMenuEvent.h"
#include "DebugPanel.h"

// TYPE | CODE | Query | Error |               Description
//------+------+-------+-------+-------------------------------------------------------------------------------------
//   0  |  0   |   *   |       |  Echo Reply——回显应答（Ping应答）
//   3  |  0   |       |   *   |  Network Unreachable——网络不可达
//   3  |  1   |       |   *   |  Host Unreachable——主机不可达
//   3  |  2   |       |   *   |  Protocol Unreachable——协议不可达
//   3  |  3   |       |   *   |  Port Unreachable——端口不可达
//   3  |  4   |       |   *   |  Fragmentation needed but no frag.bit set——需要进行分片但设置不分片比特
//   3  |  5   |       |   *   |  Source routing failed——源站选路失败
//   3  |  6   |       |   *   |  Destination network unknown——目的网络未知
//   3  |  7   |       |   *   |  Destination host unknown——目的主机未知
//   3  |  8   |       |   *   |  Source host isolated(obsolete)——源主机被隔离（作废不用）
//   3  |  9   |       |   *   |  Destination network administratively prohibited——目的网络被强制禁止
//   3  |  10  |       |   *   |  Destination host administratively prohibited——目的主机被强制禁止
//   3  |  11  |       |   *   |  Network unreachable for TOS——由于服务类型TOS，网络不可达
//   3  |  12  |       |   *   |  Host unreachable for TOS——由于服务类型TOS，主机不可达
//   3  |  13  |       |   *   |  Communication administratively prohibited by filtering——由于过滤，通信被强制禁止
//   3  |  14  |       |   *   |  Host precedence violation——主机越权
//   3  |  15  |       |   *   |  Precedence cutoff in effect——优先中止生效
//   4  |  0   |       |       |  Source quench——源端被关闭（基本流控制）
//   5  |  0   |       |       |  Redirect for network——对网络重定向
//   5  |  1   |       |       |  Redirect for host——对主机重定向
//   5  |  2   |       |       |  Redirect for TOS and network——对服务类型和网络重定向
//   5  |  3   |       |       |  Redirect for TOS and host——对服务类型和主机重定向
//   8  |  0   |   *   |       |  Echo request——回显请求（Ping请求）
//   9  |  0   |       |       |  Router advertisement——路由器通告
//  10  |  0   |       |       |  Route solicitation——路由器请求
//  11  |  0   |       |   *   |  TTL equals 0 during transit——传输期间生存时间为0
//  11  |  1   |       |   *   |  TTL equals 0 during reassembly——在数据报组装期间生存时间为0
//  12  |  0   |       |   *   |  IP header bad(catchall error)——坏的IP首部（包括各种差错）
//  12  |  1   |       |   *   |  Required options missing——缺少必需的选项
//  13  |  0   |   *   |       |  Timestamp request(obsolete)——时间戳请求（作废不用）
//  14  |      |   *   |       |  Timestamp reply(obsolete)——时间戳应答（作废不用）
//  15  |  0   |   *   |       |  Information request(obsolete)——信息请求（作废不用）
//  16  |  0   |   *   |       |  Information reply(obsolete)——信息应答（作废不用）
//  17  |  0   |   *   |       |  Address mask request——地址掩码请求
//  18  |  0   |       |       |  Address mask reply——地址掩码应答
//--------------------------------------------------------------------------------------------------------------------

// ICMP Type 表：
//  类别代号          类别名称与意义
//----------+--------------------------------------------------------------------------------------------------------------------
//     0    |  Echo Reply(代表一個回應信息)
//     3    |  Destination Unreachable(表示目的地不可到達)
//     4    |  Source Quench(當 router 的負載過高時，此類別碼可用來讓發送端停止發送訊息)
//     5    |  Redirect(用來重新導向路由路徑的資訊)
//     8    |  Echo Request(請求回應訊息)
//    11    |  Time Exceeded for a Datagram(當資料封包在某些路由傳送的現象中造成逾時狀態，此類別碼可告知來源該封包已被忽略的訊息)
//    12    |  Parameter Problem on a Datagram(當一個 ICMP 封包重複之前的錯誤時，會回覆來源主機關於參數錯誤的訊息)
//    13    |  Timestamp Request(要求對方送出時間訊息，用以計算路由時間的差異，以滿足同步性協定的要求)
//    14    |  Timestamp Reply(此訊息純粹是回應 Timestamp Request 用的)
//    15    |  Information Request(在 RARP 協定應用之前，此訊息是用來在開機時取得網路信息)
//    16    |  Information Reply(用以回應 Infromation Request 訊息)
//    17    |  Address Mask Request(這訊息是用來查詢子網路 mask 設定信息)
//    18    |  Address Mask Reply(回應子網路 mask 查詢訊息的)
//---------------------------------------------------------------------------------------------------------------------------------


/***************************************************************************
            请求回显或回显应答(Echo or Echo Reply Message)
    -----------------------------------------------------------------
     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |      Type     |      Code     |           Checksum            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            Identifier         |        Sequence Number        |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                         Optional Data                         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

***************************************************************************/

// ICMP 协议包定义

DebugInfoNetworkWidget::DebugInfoNetworkWidget(QWidget *parent)
    : DebugInfoBaseWidget(parent)
    , ui(new Ui::DebugInfoNetworkWidget())
{
    ui->setupUi(this);

    this->InitializeNetworkEnvironment();
}

DebugInfoNetworkWidget::~DebugInfoNetworkWidget()
{
    delete ui;
    this->UninitializeNetworkEnvironment();
}

void DebugInfoNetworkWidget::InitializeNetworkEnvironment()
{
    WSADATA wsaData;
    int errorCode = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (errorCode == ERROR_SUCCESS)
    {
        LogUtil::Debug(CODE_LOCATION, "------------- WSAStartup Begin -------------");
        LogUtil::Debug(CODE_LOCATION, "Expect Win Socket: %d.%d", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
        LogUtil::Debug(CODE_LOCATION, "Highest Win Socket: %d.%d", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
        //LogUtil::Debug(CODE_LOCATION, "Maximum Opened Socket: %u", wsaData.iMaxSockets); // This member is ignored for Windows Sockets version 2 and later
        //LogUtil::Debug(CODE_LOCATION, "Maximum Datagram Message Size: %u", wsaData.iMaxUdpDg); // This member is ignored for Windows Sockets version 2 and later
        //LogUtil::Debug(CODE_LOCATION, "VendorInfo: %s", wsaData.lpVendorInfo); // This member should be ignored for Windows Sockets version 2 and later
        LogUtil::Debug(CODE_LOCATION, "Description: %s", wsaData.szDescription);
        LogUtil::Debug(CODE_LOCATION, "System Status: %s", wsaData.szSystemStatus);
        LogUtil::Debug(CODE_LOCATION, "------------- WSAStartup End -------------");
    }
    else
    {
        LogUtil::Error(CODE_LOCATION, "WSAStartup failed: ErrorCode = %d", errorCode);
    }
}

void DebugInfoNetworkWidget::UninitializeNetworkEnvironment()
{
    ::WSACleanup();
}

void DebugInfoNetworkWidget::PingTest(const QString& serverIP, const QString& serverPort)
{
    struct sockaddr_in addr = { 0 };
    struct sockaddr_in saddr = { 0 };
    char sendBuffer[64] = { 0 };
    char recvBuffer[512] = { 0 };
    int retValue;
    int addressLength = 0;
    int repeatCount = 5;
    int i = 1;

    SOCKET client = ::socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (client == INVALID_SOCKET)
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "Create ICMP client socket failed: errorCode = %d", errorCode);
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.13.148");

    while (repeatCount > 0)
    {
        --repeatCount;

        memset(sendBuffer, 0, sizeof(sendBuffer));
        //icmp_packet(sendBuffer, 64, getpid(), i);
        i++;

        retValue = ::sendto(client, sendBuffer, 64, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (retValue <= 0)
        {
            LogUtil::Error(CODE_LOCATION, "Sent ICMP data failed!");
            break;
        }
        else
        {
            LogUtil::Info(CODE_LOCATION, "Send %d bytes ...", retValue);
        }

        memset(recvBuffer, 0, sizeof(recvBuffer));
        memset(&saddr, 0, sizeof(saddr));
        addressLength = sizeof(saddr);
        retValue = recvfrom(client, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)&saddr, &addressLength);
        if (retValue <= 0)
        {
            LogUtil::Error(CODE_LOCATION, "Send ICMP data failed!");
            break;
        }
        else
        {
            LogUtil::Info(CODE_LOCATION, "Receive %d bytes ...", retValue);
        }
        //parse_packet(recvBuffer, retValue);
        ::Sleep(1);
    }

    int errorCode = ::closesocket(client);
    if (errorCode != ERROR_SUCCESS)
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "Close client socket failed: errorCode = %d", errorCode);
        return;
    }
}

bool DebugInfoNetworkWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    int menuID = event->GetMenuID();
    QMetaEnum menuIDEnum = QMetaEnum::fromType<DebugPanel::MenuID>();
    LogUtil::Info(CODE_LOCATION, "Do not specify MenuID = %s handler ...", menuIDEnum.valueToKey(menuID));

    return true;
}

void DebugInfoNetworkWidget::OnUpdateDebugInfo()
{
    ;
}

void DebugInfoNetworkWidget::resizeEvent(QResizeEvent *event)
{
    ;
}

void DebugInfoNetworkWidget::on_btnPingTest_clicked()
{
    int i = 0;
}
