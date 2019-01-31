#include "DebugInfoNetworkWidget.h"
#include "ui_DebugInfoNetworkWidget.h"

// QT Headers
#include <QMetaEnum>

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

/***************************************************************************
                                IP 头部协议格式
    -----------------------------------------------------------------
     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  Ver  | HdLen | TypeOfService |         Total Length          |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            Identifier         |  F  |     Fragment Offset     |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |      TTL      |    Protocol   |        Header Checksum        |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                      Source IP Address                        |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                    Destination IP Address                     |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                      Option And Padding                       |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

***************************************************************************/

//                       IP 头部协议说明
//  Version (协议版本)
//      占 4 比特，表名 IP 协议版本。IPv4->0100，IPv6->0110。
//
//  Header Length （报文头部长度）
//      占 4 比特，表示占 多少个 32 比特，包括变长可选项。
//      普通 IP 数据包无可选项，改值为 5， 即 5 个 32 比特, 160比特 = 4字节 x 5 = 20字节。
//      IP 头部长度范围 20 ~ 60 字节，可选项长度 0 ~ 40 字节。
//
//  Type of Service （服务类型）
//      占 8 比特，指示服务类型。
//      第 1~3 比特为优先权，现已忽略。
//            000 普通 (Routine)
//            001 优先的(Priority)
//            010 立即的发送(Immediate)
//            011 闪电式的(Flash)
//            100 超闪电式的(Flash Override)
//            101 CRI/TIC/ECP
//            110 网间控制(Internetwork Control)
//            111 网络控制(Network Control)
//      第 4~7 比特分别表示：延迟、吞吐量、可靠性、花费。
//            值为 1 时代表：最小时延、最大吞吐量、最高可靠性、最小费用。
//            互斥，只能置其中 1 比特为 1，可以全为 0，表示一般服务。
//      第 8 比特保留，恒为 0。
//      eg: TELNET->最小延时(D)、FTP->最大吞吐量(T)、SNMP->最高可靠性(R)、NNTP->最小费用(M)、ICMP->无特殊需求（全为0）。
//
//  Total Length （报文总长度）
//      占 16 比特，指明整个数据包的长度，以字节为单位。 <= 65535 字节
//
//  Identifier
//      占 16 比特，唯一标识主机发送的每一分数据报。通常每发一份报文，其值加 1。
//      与 Flags 和 Fragment Offset 联合使用，所有拆分开的小包标记相同的值，以便目的端区分小包属于哪个大包。
//
//  Flags (标志位)
//      占 3 比特，标识数据报是否要分段。
//      第 1 比特，不使用。
//      第 2 比特，DF（Don't Fragment）位，为 1 时，表明路由器不能对该上层数据包分段。
//                如果上层数据包无法在不分段的情况下转发，则丢弃该上层数据包并返回一条错误信息。
//      第 3 比特，MF(More Fragment)位，路由器对上层数据包分段时，除最后一个分段外其余分段的该标记位都设为 1。
//
//  Fragment Offset （段偏移）
//      占 13 比特，如果数据报需要分段，该字段指明该段报文距原始数据报开始位置的偏移量。
//
//  Time To Live （生存期）
//      占 8 比特，设置报文最多可以经过的路由器数。
//      由发送数据的源主机设置，通常为 32、64、128 等。
//      没经过一个路由器，其值减 1，直到为 0 时数据包被丢弃。
//
//  Protocol （协议）
//      占 8 比特，指明所封装的上层协议类型。
//      如：ICMP（1）、IGMP（2）、TCP（6）、UDP（17）、IGRP（88）、OSPF（89）等。
//
//  Header Checksum （头部校验和）
//      占 16 比特，注意仅对 IP 头部数据进行校验。
//      对头部中每个 16 比特进行二进制反码求和。
//
//  Source Address （源 IP 地址）
//      占 32 比特，注意网络字节序。
//
//  Destination Address （目的 IP 地址）
//      占 32 比特，注意网络字节序。
//
//  Option and Padding （可选项）
//      占 32 比特，定义一些人选项：如记录路径、时间戳等。并非所有主机和路由器都支持这些选项。
//      可选项长度必需是 32 比特的整数倍，如果不足，用 0 填充。
//      可选项包含以下内容：
//          松散源路由（Loose Source Routing）：给出一连串路由器地址。数据包必须沿着这些 IP 地址传递，允许跳过多个路由器。
//          严格源路由（Strict Source Routing）：给出一连串路由器地址。数据包必须沿着这些 IP 地址传送，下一跳不在 IP 地址表中则发生错误。
//          路由记录（Record Route）：当数据包离开每个路由器的时候记录路由器出站 IP 地址。
//          时间戳（Timestamps）：当数据包离开每个路由器的时候记录时间。


// ICMP 协议包定义
struct ICMP_HEADER
{
    unsigned char  Type;
    unsigned char  Code;
    unsigned short Checksum;
    unsigned short Identifier;
    unsigned short Sequence;
    unsigned long  OptinalData;
};

struct IP_HEADER
{
    unsigned char  HeaderLength:4;    //length of header
    unsigned char  Version:4;         //Version of IP
    unsigned char  TypeOfService;     //Type of service
    unsigned short PacketLength;      //total length of the packet
    unsigned short PacketID;          //unique identifier
    unsigned short SliceInfo;         //flags
    unsigned char  TimeToLive;        //ttl
    unsigned char  TypeOfProtocol;    //protocol(TCP ,UDP etc)
    unsigned short Checksum;          //IP checksum
    unsigned int   SourceIP;
    unsigned int   DestinationIP;
};

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

void DebugInfoNetworkWidget::PingTest(const QString& destinationAddress)
{
    struct sockaddr_in sendAddress = { 0 };
    struct sockaddr_in recvAddress = { 0 };
    char sendBuffer[64] = { 0 };
    char recvBuffer[512] = { 0 };
    int retValue;
    int addressLength = 0;
    int repeatCount = 5;
    int sequence = 1;

    hostent *pHost = gethostbyname(destinationAddress.toStdString().c_str());
    if (pHost == NULL)
    {
        LogUtil::Error(CODE_LOCATION, "Bad destination address.");
        return;
    }
    
    LogUtil::Debug(CODE_LOCATION, "Destination info:");
    LogUtil::Debug(CODE_LOCATION, "            Type: %u", pHost->h_addrtype);
    LogUtil::Debug(CODE_LOCATION, "          Length: %u", pHost->h_length);
    LogUtil::Debug(CODE_LOCATION, "            name: %s", pHost->h_name);
    LogUtil::Debug(CODE_LOCATION, "         IP Addr: %s", inet_ntoa(*((struct in_addr*)pHost->h_addr)));

    char **pptr = NULL;
    int index = 1;
    for (pptr = pHost->h_aliases; *pptr != NULL; pptr++, index++)
    {
        LogUtil::Debug(CODE_LOCATION, "       Alias[%d]: %s", index, *pptr);
    }

    SOCKET client = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (client == INVALID_SOCKET)
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "Create ICMP client socket failed: errorCode = %d", errorCode);
        return;
    }

    sendAddress.sin_family = AF_INET;
    //sendAddress.sin_addr.s_addr = inet_addr("10.225.12.64");
    sendAddress.sin_addr = *((struct in_addr*)pHost->h_addr);
    //sendAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (repeatCount > 0)
    {
        memset(sendBuffer, 0, sizeof(sendBuffer));
        ICMP_HEADER *pSendICMP = (ICMP_HEADER*)sendBuffer;
        pSendICMP->Type = 8; // Echo Request
        pSendICMP->Code = 0;
        pSendICMP->Checksum = 0;
        pSendICMP->Identifier = GetCurrentProcessId();
        pSendICMP->Sequence = sequence;
        pSendICMP->OptinalData = GetTickCount();
        pSendICMP->Checksum = this->CaculateChecksum((UINT8*)sendBuffer, sizeof(ICMP_HEADER));

        --repeatCount;
        sequence++;

        retValue = sendto(client, sendBuffer, 64, 0, (struct sockaddr *)&sendAddress, sizeof(sendAddress));
        if (retValue <= 0)
        {
            LogUtil::Error(CODE_LOCATION, "Sent ICMP data failed!");
            break;
        }
        else
        {
            //LogUtil::Debug(CODE_LOCATION, "Send %d bytes ...", retValue);
        }

        memset(recvBuffer, 0, sizeof(recvBuffer));
        memset(&recvAddress, 0, sizeof(recvAddress));
        addressLength = sizeof(recvAddress);
        retValue = recvfrom(client, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)&recvAddress, &addressLength);
        if (retValue <= 0)
        {
            LogUtil::Error(CODE_LOCATION, "Send ICMP data failed!");
            break;
        }
        else
        {
            //LogUtil::Debug(CODE_LOCATION, "Receive %d bytes ...", retValue);
            IP_HEADER *pRecvIP = (IP_HEADER*)recvBuffer;
            ICMP_HEADER *pRecvICMP = (ICMP_HEADER*)(pRecvIP + 1);
            unsigned long timeDiff = GetTickCount() - pRecvICMP->OptinalData;
            LogUtil::Debug(CODE_LOCATION, "Receive ICMP Type:%u Code:%u", pRecvICMP->Type, pRecvICMP->Code);

            UINT16 checksum = this->CaculateChecksum((UINT8*)pRecvICMP, sizeof(ICMP_HEADER));
            if (checksum == 0)
            {
                LogUtil::Info(CODE_LOCATION, "Reply From %s: bytes=%d time=%ums TTL=%u",
                              inet_ntoa(recvAddress.sin_addr),
                              retValue, timeDiff, pRecvIP->TimeToLive);
            }
            else
            {
                LogUtil::Error(CODE_LOCATION, "Checksum Error");
            }
        }
        Sleep(1);
    }

    int errorCode = closesocket(client);
    if (errorCode != ERROR_SUCCESS)
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "Close client socket failed: errorCode = %d", errorCode);
        return;
    }
}

UINT16 DebugInfoNetworkWidget::CaculateChecksum(UINT8 *InBuffer, INT32 BufferLen)
{
    UINT32 sum = 0;
    UINT16 *tempBuffer;

    tempBuffer = (UINT16 *)InBuffer;

    while (BufferLen> 1)
    {
        sum += *tempBuffer++;
        BufferLen -= 2;
    }

    if (BufferLen)
    {
        sum += *(UINT8 *)tempBuffer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return ~sum;
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
    int i = 0;
}

void DebugInfoNetworkWidget::on_btnPingTest_clicked()
{
    QString serverAddress = ui->leDestinationAddress->text();
    this->PingTest(serverAddress);
}
