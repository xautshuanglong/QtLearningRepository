#include "DebugInfoNetworkWidget.h"
#include "ui_DebugInfoNetworkWidget.h"

// QT Headers
#include <QMetaEnum>

// Windows Headers
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <IcmpAPI.h>

// Self Headers
#include <LogUtil.h>
#include "DebugMenuEvent.h"
#include "DebugPanel.h"

// TYPE | CODE | Query | Error |               Description
//------+------+-------+-------+-------------------------------------------------------------------------------------
//   0  |  0   |   *   |       |  Echo Reply��������Ӧ��PingӦ��
//   3  |  0   |       |   *   |  Network Unreachable�������粻�ɴ�
//   3  |  1   |       |   *   |  Host Unreachable�����������ɴ�
//   3  |  2   |       |   *   |  Protocol Unreachable����Э�鲻�ɴ�
//   3  |  3   |       |   *   |  Port Unreachable�����˿ڲ��ɴ�
//   3  |  4   |       |   *   |  Fragmentation needed but no frag.bit set������Ҫ���з�Ƭ�����ò���Ƭ����
//   3  |  5   |       |   *   |  Source routing failed����Դվѡ·ʧ��
//   3  |  6   |       |   *   |  Destination network unknown����Ŀ������δ֪
//   3  |  7   |       |   *   |  Destination host unknown����Ŀ������δ֪
//   3  |  8   |       |   *   |  Source host isolated(obsolete)����Դ���������루���ϲ��ã�
//   3  |  9   |       |   *   |  Destination network administratively prohibited����Ŀ�����类ǿ�ƽ�ֹ
//   3  |  10  |       |   *   |  Destination host administratively prohibited����Ŀ��������ǿ�ƽ�ֹ
//   3  |  11  |       |   *   |  Network unreachable for TOS�������ڷ�������TOS�����粻�ɴ�
//   3  |  12  |       |   *   |  Host unreachable for TOS�������ڷ�������TOS���������ɴ�
//   3  |  13  |       |   *   |  Communication administratively prohibited by filtering�������ڹ��ˣ�ͨ�ű�ǿ�ƽ�ֹ
//   3  |  14  |       |   *   |  Host precedence violation��������ԽȨ
//   3  |  15  |       |   *   |  Precedence cutoff in effect����������ֹ��Ч
//   4  |  0   |       |       |  Source quench����Դ�˱��رգ����������ƣ�
//   5  |  0   |       |       |  Redirect for network�����������ض���
//   5  |  1   |       |       |  Redirect for host�����������ض���
//   5  |  2   |       |       |  Redirect for TOS and network�����Է������ͺ������ض���
//   5  |  3   |       |       |  Redirect for TOS and host�����Է������ͺ������ض���
//   8  |  0   |   *   |       |  Echo request������������Ping����
//   9  |  0   |       |       |  Router advertisement����·����ͨ��
//  10  |  0   |       |       |  Route solicitation����·��������
//  11  |  0   |       |   *   |  TTL equals 0 during transit���������ڼ�����ʱ��Ϊ0
//  11  |  1   |       |   *   |  TTL equals 0 during reassembly���������ݱ���װ�ڼ�����ʱ��Ϊ0
//  12  |  0   |       |   *   |  IP header bad(catchall error)��������IP�ײ����������ֲ��
//  12  |  1   |       |   *   |  Required options missing����ȱ�ٱ����ѡ��
//  13  |  0   |   *   |       |  Timestamp request(obsolete)����ʱ����������ϲ��ã�
//  14  |      |   *   |       |  Timestamp reply(obsolete)����ʱ���Ӧ�����ϲ��ã�
//  15  |  0   |   *   |       |  Information request(obsolete)������Ϣ�������ϲ��ã�
//  16  |  0   |   *   |       |  Information reply(obsolete)������ϢӦ�����ϲ��ã�
//  17  |  0   |   *   |       |  Address mask request������ַ��������
//  18  |  0   |       |       |  Address mask reply������ַ����Ӧ��
//--------------------------------------------------------------------------------------------------------------------

// ICMP Type ��
//  ������          �������������
//----------+--------------------------------------------------------------------------------------------------------------------
//     0    |  Echo Reply(����һ���ؑ���Ϣ)
//     3    |  Destination Unreachable(��ʾĿ�ĵز��ɵ��_)
//     4    |  Source Quench(�� router ��ؓ�d�^�ߕr����e�a���Á�׌�l�Ͷ�ֹͣ�l��ӍϢ)
//     5    |  Redirect(�Á�������·��·�����YӍ)
//     8    |  Echo Request(Ո��ؑ�ӍϢ)
//    11    |  Time Exceeded for a Datagram(���Y�Ϸ����ĳЩ·�ɂ��͵ĬF���������r��B����e�a�ɸ�֪��Դԓ����ѱ����Ե�ӍϢ)
//    12    |  Parameter Problem on a Datagram(��һ�� ICMP ������}֮ǰ���e�`�r�����ظ���Դ���C�P춅����e�`��ӍϢ)
//    13    |  Timestamp Request(Ҫ�󌦷��ͳ��r�gӍϢ������Ӌ��·�ɕr�g�Ĳ���ԝM��ͬ���ԅf����Ҫ��)
//    14    |  Timestamp Reply(��ӍϢ�����ǻؑ� Timestamp Request �õ�)
//    15    |  Information Request(�� RARP �f������֮ǰ����ӍϢ���Á����_�C�rȡ�þW·��Ϣ)
//    16    |  Information Reply(���Իؑ� Infromation Request ӍϢ)
//    17    |  Address Mask Request(�@ӍϢ���Á��ԃ�ӾW· mask �O����Ϣ)
//    18    |  Address Mask Reply(�ؑ��ӾW· mask ��ԃӍϢ��)
//---------------------------------------------------------------------------------------------------------------------------------

/***************************************************************************
            ������Ի����Ӧ��(Echo or Echo Reply Message)
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
                                IP ͷ��Э���ʽ
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

//                       IP ͷ��Э��˵��
//  Version (Э��汾)
//      ռ 4 ���أ����� IP Э��汾��IPv4->0100��IPv6->0110��
//
//  Header Length ������ͷ�����ȣ�
//      ռ 4 ���أ���ʾռ ���ٸ� 32 ���أ������䳤��ѡ�
//      ��ͨ IP ���ݰ��޿�ѡ���ֵΪ 5�� �� 5 �� 32 ����, 160���� = 4�ֽ� x 5 = 20�ֽڡ�
//      IP ͷ�����ȷ�Χ 20 ~ 60 �ֽڣ���ѡ��� 0 ~ 40 �ֽڡ�
//
//  Type of Service ���������ͣ�
//      ռ 8 ���أ�ָʾ�������͡�
//      �� 1~3 ����Ϊ����Ȩ�����Ѻ��ԡ�
//            000 ��ͨ (Routine)
//            001 ���ȵ�(Priority)
//            010 �����ķ���(Immediate)
//            011 ����ʽ��(Flash)
//            100 ������ʽ��(Flash Override)
//            101 CRI/TIC/ECP
//            110 �������(Internetwork Control)
//            111 �������(Network Control)
//      �� 4~7 ���طֱ��ʾ���ӳ١����������ɿ��ԡ����ѡ�
//            ֵΪ 1 ʱ������Сʱ�ӡ��������������߿ɿ��ԡ���С���á�
//            ���⣬ֻ�������� 1 ����Ϊ 1������ȫΪ 0����ʾһ�����
//      �� 8 ���ر�������Ϊ 0��
//      eg: TELNET->��С��ʱ(D)��FTP->���������(T)��SNMP->��߿ɿ���(R)��NNTP->��С����(M)��ICMP->����������ȫΪ0����
//
//  Total Length �������ܳ��ȣ�
//      ռ 16 ���أ�ָ���������ݰ��ĳ��ȣ����ֽ�Ϊ��λ�� <= 65535 �ֽ�
//
//  Identifier
//      ռ 16 ���أ�Ψһ��ʶ�������͵�ÿһ�����ݱ���ͨ��ÿ��һ�ݱ��ģ���ֵ�� 1��
//      �� Flags �� Fragment Offset ����ʹ�ã����в�ֿ���С�������ͬ��ֵ���Ա�Ŀ�Ķ�����С�������ĸ������
//
//  Flags (��־λ)
//      ռ 3 ���أ���ʶ���ݱ��Ƿ�Ҫ�ֶΡ�
//      �� 1 ���أ���ʹ�á�
//      �� 2 ���أ�DF��Don't Fragment��λ��Ϊ 1 ʱ������·�������ܶԸ��ϲ����ݰ��ֶΡ�
//                ����ϲ����ݰ��޷��ڲ��ֶε������ת�����������ϲ����ݰ�������һ��������Ϣ��
//      �� 3 ���أ�MF(More Fragment)λ��·�������ϲ����ݰ��ֶ�ʱ�������һ���ֶ�������ֶεĸñ��λ����Ϊ 1��
//
//  Fragment Offset ����ƫ�ƣ�
//      ռ 13 ���أ�������ݱ���Ҫ�ֶΣ����ֶ�ָ���öα��ľ�ԭʼ���ݱ���ʼλ�õ�ƫ������
//
//  Time To Live �������ڣ�
//      ռ 8 ���أ����ñ��������Ծ�����·��������
//      �ɷ������ݵ�Դ�������ã�ͨ��Ϊ 32��64��128 �ȡ�
//      û����һ��·��������ֵ�� 1��ֱ��Ϊ 0 ʱ���ݰ���������
//
//  Protocol ��Э�飩
//      ռ 8 ���أ�ָ������װ���ϲ�Э�����͡�
//      �磺ICMP��1����IGMP��2����TCP��6����UDP��17����IGRP��88����OSPF��89���ȡ�
//
//  Header Checksum ��ͷ��У��ͣ�
//      ռ 16 ���أ�ע����� IP ͷ�����ݽ���У�顣
//      ��ͷ����ÿ�� 16 ���ؽ��ж����Ʒ�����͡�
//
//  Source Address ��Դ IP ��ַ��
//      ռ 32 ���أ�ע�������ֽ���
//
//  Destination Address ��Ŀ�� IP ��ַ��
//      ռ 32 ���أ�ע�������ֽ���
//
//  Option and Padding ����ѡ�
//      ռ 32 ���أ�����һЩ��ѡ����¼·����ʱ����ȡ���������������·������֧����Щѡ�
//      ��ѡ��ȱ����� 32 ���ص���������������㣬�� 0 ��䡣
//      ��ѡ������������ݣ�
//          ��ɢԴ·�ɣ�Loose Source Routing��������һ����·������ַ�����ݰ�����������Щ IP ��ַ���ݣ������������·������
//          �ϸ�Դ·�ɣ�Strict Source Routing��������һ����·������ַ�����ݰ�����������Щ IP ��ַ���ͣ���һ������ IP ��ַ������������
//          ·�ɼ�¼��Record Route���������ݰ��뿪ÿ��·������ʱ���¼·������վ IP ��ַ��
//          ʱ�����Timestamps���������ݰ��뿪ÿ��·������ʱ���¼ʱ�䡣


// ICMP Э�������
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

    // �������� IPv4
    struct hostent *pHost = gethostbyname(destinationAddress.toStdString().c_str());
    if (pHost == NULL)
    {
        int errorCode = WSAGetLastError();
        switch (errorCode)
        {
        case WSAHOST_NOT_FOUND:
            LogUtil::Error(CODE_LOCATION, "Host not found ...");
            break;
        case WSANO_DATA:
            LogUtil::Error(CODE_LOCATION, "No data record found ...");
            break;
        default:
            LogUtil::Error(CODE_LOCATION, "Bad destination address. ErrorCode: %d", errorCode);
            break;
        }
        return;
    }

    this->ParseHostInfo(pHost);

    SOCKET client = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (client == INVALID_SOCKET)
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "Create ICMP client socket failed: errorCode = %d", errorCode);
        return;
    }
    unsigned long sendTimeout = 10000;
    retValue = setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (char*)&sendTimeout, sizeof(unsigned long));
    if (retValue != ERROR_SUCCESS)
    {
        LogUtil::Error(CODE_LOCATION, "Failed setsockopt SO_SNDTIMEO: ErrorCode = %d", WSAGetLastError());
    }
    unsigned long receiveTimeout = 5000;
    retValue = setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&receiveTimeout, sizeof(unsigned long));
    if (retValue != ERROR_SUCCESS)
    {
        LogUtil::Error(CODE_LOCATION, "Failed setsockopt SO_RCVTIMEO: ErrorCode = %d", WSAGetLastError());
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
        pSendICMP->Sequence = sequence++;
        pSendICMP->OptinalData = GetTickCount();
        pSendICMP->Checksum = this->CaculateChecksum((UINT8*)sendBuffer, sizeof(ICMP_HEADER));

        retValue = sendto(client, sendBuffer, 64, 0, (struct sockaddr *)&sendAddress, sizeof(sendAddress));
        if (retValue <= 0)
        {
            LogUtil::Error(CODE_LOCATION, "Sent ICMP data failed!");
            break;
        }
        else
        {
            LogUtil::Debug(CODE_LOCATION, "Send %d bytes ...", retValue);
        }

        memset(recvBuffer, 0, sizeof(recvBuffer));
        memset(&recvAddress, 0, sizeof(recvAddress));
        addressLength = sizeof(recvAddress);
        retValue = recvfrom(client, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)&recvAddress, &addressLength);
        unsigned long recvTime = GetTickCount();
        if (retValue <= 0)
        {
            int errorCode = WSAGetLastError();
            switch (errorCode)
            {
            case WSAETIMEDOUT:
                LogUtil::Error(CODE_LOCATION, "Receive ICMP data failed! ErrorCode: WSAETIMEDOUT");
            default:
                LogUtil::Error(CODE_LOCATION, "Receive ICMP data failed! ErrorCode: %d", errorCode);
                break;
            }
            break;
        }
        else
        {
            //LogUtil::Debug(CODE_LOCATION, "Receive %d bytes ...", retValue);
            IP_HEADER *pRecvIP = (IP_HEADER*)recvBuffer;
            ICMP_HEADER *pRecvICMP = (ICMP_HEADER*)(pRecvIP + 1);
            unsigned long timeDiff = recvTime - pRecvICMP->OptinalData;
            if (pRecvICMP->Type==0 && pRecvICMP->Code==0)
            {
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
                    break;
                }
            }
            else
            {
                LogUtil::Debug(CODE_LOCATION, "Receive ICMP Data: Type=%u Code=%u", pRecvICMP->Type, pRecvICMP->Code);
            }
        }

        --repeatCount;
        Sleep(1);
    }

    retValue = closesocket(client);
    if (retValue != ERROR_SUCCESS)
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "Close client socket failed: ErrorCode = %d", errorCode);
        return;
    }
}

void DebugInfoNetworkWidget::WinAPIGetAddrInfoTest(const QString& destinationAddress)
{
    DWORD dwRetval;
    INT iRetval;
    int iResult;
    int i = 1;

    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;

    struct sockaddr_in  *sockaddr_ipv4;
    struct sockaddr_in6 *sockaddr_ipv6;
    LPSOCKADDR sockaddr_ip;

    char ipstringbuffer[46];
    DWORD ipbufferlength = 46;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    dwRetval = getaddrinfo(destinationAddress.toStdString().c_str(), "", &hints, &result); // 10.225.12.64
    if (dwRetval != ERROR_SUCCESS)
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "getaddrinfo failed with error[%d]: %s", errorCode, gai_strerror(dwRetval)); // WSA_INVALID_HANDLE
        return;
    }
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        LogUtil::Debug(CODE_LOCATION, "getaddrinfo response %d", i++);
        LogUtil::Debug(CODE_LOCATION, "\tFlags: 0x%x", ptr->ai_flags);
        LogUtil::Debug(CODE_LOCATION, "\tFamily: ");
        switch (ptr->ai_family) {
        case AF_UNSPEC:
            LogUtil::Debug(CODE_LOCATION, "Unspecified");
            break;
        case AF_INET:
            LogUtil::Debug(CODE_LOCATION, "AF_INET (IPv4)");
            sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
            LogUtil::Debug(CODE_LOCATION, "\tIPv4 address %s",
                   inet_ntoa(sockaddr_ipv4->sin_addr));
            break;
        case AF_INET6:
            LogUtil::Debug(CODE_LOCATION, "AF_INET6 (IPv6)");
            sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
            LogUtil::Debug(CODE_LOCATION, "\tIPv6 address %s",
                           InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
            sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;
            ipbufferlength = 46;
            iRetval = WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
                                         ipstringbuffer, &ipbufferlength);
            if (iRetval)
                LogUtil::Debug(CODE_LOCATION, "WSAAddressToString failed with %u", WSAGetLastError());
            else
                LogUtil::Debug(CODE_LOCATION, "\tIPv6 address %s", ipstringbuffer);
            break;
        case AF_NETBIOS:
            LogUtil::Debug(CODE_LOCATION, "AF_NETBIOS (NetBIOS)");
            break;
        default:
            LogUtil::Debug(CODE_LOCATION, "Other %ld", ptr->ai_family);
            break;
        }
        LogUtil::Debug(CODE_LOCATION, "\tSocket type: ");
        switch (ptr->ai_socktype) {
        case 0:
            LogUtil::Debug(CODE_LOCATION, "Unspecified");
            break;
        case SOCK_STREAM:
            LogUtil::Debug(CODE_LOCATION, "SOCK_STREAM (stream)");
            break;
        case SOCK_DGRAM:
            LogUtil::Debug(CODE_LOCATION, "SOCK_DGRAM (datagram) ");
            break;
        case SOCK_RAW:
            LogUtil::Debug(CODE_LOCATION, "SOCK_RAW (raw) ");
            break;
        case SOCK_RDM:
            LogUtil::Debug(CODE_LOCATION, "SOCK_RDM (reliable message datagram)");
            break;
        case SOCK_SEQPACKET:
            LogUtil::Debug(CODE_LOCATION, "SOCK_SEQPACKET (pseudo-stream packet)");
            break;
        default:
            LogUtil::Debug(CODE_LOCATION, "Other %ld", ptr->ai_socktype);
            break;
        }
        LogUtil::Debug(CODE_LOCATION, "\tProtocol: ");
        switch (ptr->ai_protocol) {
        case 0:
            LogUtil::Debug(CODE_LOCATION, "Unspecified");
            break;
        case IPPROTO_TCP:
            LogUtil::Debug(CODE_LOCATION, "IPPROTO_TCP (TCP)");
            break;
        case IPPROTO_UDP:
            LogUtil::Debug(CODE_LOCATION, "IPPROTO_UDP (UDP) ");
            break;
        default:
            LogUtil::Debug(CODE_LOCATION, "Other %ld", ptr->ai_protocol);
            break;
        }
        LogUtil::Debug(CODE_LOCATION, "\tLength of this sockaddr: %d", ptr->ai_addrlen);
        LogUtil::Debug(CODE_LOCATION, "\tCanonical name: %s", ptr->ai_canonname);
    }

    freeaddrinfo(result);
}

void DebugInfoNetworkWidget::WinAPIGetHostByAddrTest(const QString &destinationAddress)
{
    int i = 0;
    int bIpv6 = 0;
    int iResult;

    struct hostent *remoteHost = NULL;
    struct in_addr addr4 = { 0 };
    IN6_ADDR addr6;
    char **pAlias;
    char ipv6AddrString[64] = { 0 };

    std::string hostAddrStr = destinationAddress.toStdString();
    const char *host_addr = hostAddrStr.c_str();

    LogUtil::Info(CODE_LOCATION, "Calling gethostbyaddr with: %s", host_addr);
    iResult = inet_pton(AF_INET6, host_addr, &addr6);
    if (iResult == 1) // returns a value of 1 and the buffer pointed to by the pAddrBuf parameter contains the binary numeric IP address in network byte order.
    {
        remoteHost = gethostbyaddr((char *)&addr6, 16, AF_INET6);
    }
    else if (iResult == 0) // returns a value of 0 if the pAddrBuf parameter points to a string that is not a valid IPv4 dotted-decimal string or a valid IPv6 address string. 
    {
        addr4.s_addr = inet_addr(host_addr);
        if (addr4.s_addr == INADDR_NONE)
        {
            LogUtil::Error(CODE_LOCATION, "The address entered is not legal address");
        }
        else
        {
            remoteHost = gethostbyaddr((char *)&addr4, 4, AF_INET);
        }
    }
    else // Otherwise, a value of -1 is returned
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "inet_pton failed: ErrorCode = %d", errorCode);
        return;
    }

    if (remoteHost == NULL)
    {
        int errorCode = WSAGetLastError();
        switch (errorCode)
        {
        case WSAHOST_NOT_FOUND:
            LogUtil::Error(CODE_LOCATION, "Host not found");
            break;
        case WSANO_DATA:
            LogUtil::Error(CODE_LOCATION, "No data record found");
            break;
        default:
            LogUtil::Error(CODE_LOCATION, "Function failed with error: %d", errorCode);
            break;
        }
    }
    else
    {
        LogUtil::Info(CODE_LOCATION, "Function returned:                  ");
        LogUtil::Info(CODE_LOCATION, "\tOfficial name: %s", remoteHost->h_name);
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++)
        {
            LogUtil::Info(CODE_LOCATION, "\tAlternate name #%d: %s", ++i, *pAlias);
        }
        LogUtil::Info(CODE_LOCATION, "\tAddress type:                    ");
        switch (remoteHost->h_addrtype)
        {
        case AF_INET:
            LogUtil::Info(CODE_LOCATION, "AF_INET");
            break;
        case AF_INET6:
            LogUtil::Info(CODE_LOCATION, "AF_INET6");
            break;
        case AF_NETBIOS:
            LogUtil::Info(CODE_LOCATION, "AF_NETBIOS");
            break;
        default:
            LogUtil::Info(CODE_LOCATION, "Unknown address type�� %d", remoteHost->h_addrtype);
            break;
        }
        LogUtil::Info(CODE_LOCATION, "\tAddress length: %d", remoteHost->h_length);

        if (remoteHost->h_addrtype == AF_INET)
        {
            i = 0;
            while (remoteHost->h_addr_list[i] != 0)
            {
                addr4.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
                LogUtil::Info(CODE_LOCATION, "\tIPv4 Address #%d: %s", i, inet_ntoa(addr4));
            }
        }
        else if (remoteHost->h_addrtype == AF_INET6)
        {
            i = 0;
            while (remoteHost->h_addr_list[i] != 0)
            {
                LogUtil::Info(CODE_LOCATION, "\tIPv6 Address #%d: %s", i, inet_ntop(AF_INET6, remoteHost->h_addr_list[i++], ipv6AddrString, sizeof(ipv6AddrString)));
            }
        }
    }
}

void DebugInfoNetworkWidget::WinAPIGetNameInfoTest(const QString &destinationAddress, const int &destinationPort)
{
    int iResult = 0;
    DWORD dwRetval;

    struct sockaddr_in saGNI;
    char hostname[NI_MAXHOST];
    char servInfo[NI_MAXSERV];
    u_short port = destinationPort;

    saGNI.sin_family = AF_INET;
    saGNI.sin_addr.s_addr = inet_addr(destinationAddress.toStdString().c_str());
    saGNI.sin_port = htons(port);

    // NI_NOFQDN       0x01  /* Only return nodename portion for local hosts */
    // NI_NUMERICHOST  0x02  /* Return numeric form of the host's address */
    // NI_NAMEREQD     0x04  /* Error if the host's name not in DNS */
    // NI_NUMERICSERV  0x08  /* Return numeric form of the service (port #) */
    // NI_DGRAM        0x10  /* Service is a datagram service */

    const int flags[5] =
    {
        NI_NAMEREQD,
        NI_NOFQDN,
        NI_NUMERICHOST,
        NI_NUMERICSERV,
        NI_DGRAM
    };
    int flagIndex = 0;

    do 
    {
        dwRetval = getnameinfo((struct sockaddr *) &saGNI, sizeof(struct sockaddr), hostname,
                               NI_MAXHOST, servInfo, NI_MAXSERV, flags[flagIndex]);
        if (dwRetval != 0)
        {
            int errorCode = WSAGetLastError();
            LogUtil::Error(CODE_LOCATION, "Failed with error[%d]: %d", flagIndex, errorCode);
        }
        ++flagIndex;
    } while (dwRetval!=0 && flagIndex<5);

    if (dwRetval != 0)
    {
        int errorCode = WSAGetLastError();
        LogUtil::Error(CODE_LOCATION, "getnameinfo failed with error: %d", errorCode);
    }
    else
    {
        LogUtil::Debug(CODE_LOCATION, "Hostname: %s | %s", hostname, servInfo);
    }
}

void DebugInfoNetworkWidget::WinAPIIcmpSendEchoTest(const QString &destinationAddress)
{
    HANDLE hIcmpFile = INVALID_HANDLE_VALUE;
    IPAddr ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;
    DWORD errorCode = ERROR_SUCCESS;

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE)
    {
        errorCode = GetLastError();
        LogUtil::Error(CODE_LOCATION, "Unable to open handle. ErrorCode: %u", errorCode);
        return;
    }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (void*)malloc(ReplySize);
    if (ReplyBuffer == NULL)
    {
        LogUtil::Error(CODE_LOCATION, "Unable to allocate memory");
        return;
    }

    ipaddr = inet_addr(destinationAddress.toStdString().c_str());
    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
                            NULL, ReplyBuffer, ReplySize, 1000);
    if (dwRetVal != 0)
    {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;

        LogUtil::Info(CODE_LOCATION, "Sent icmp message to %s", destinationAddress.toStdString().c_str());
        LogUtil::Info(CODE_LOCATION, "Received %ld icmp message responses", dwRetVal);
        LogUtil::Info(CODE_LOCATION, "\t  Received from %s", inet_ntoa(ReplyAddr));
        LogUtil::Info(CODE_LOCATION, "\t  Roundtrip time = %lu milliseconds", pEchoReply->RoundTripTime);
        LogUtil::Info(CODE_LOCATION, "\t  Time to live = %u", pEchoReply->Options.Ttl);
        if (pEchoReply->Status == IP_SUCCESS)
        {
            LogUtil::Info(CODE_LOCATION, "\t  Status = %ld (IP_SUCCESS) %s", pEchoReply->Status, pEchoReply->Data);
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "\t  Failed with status code: %lu", pEchoReply->Status);
        }
    }
    else
    {
        errorCode = GetLastError();
        LogUtil::Error(CODE_LOCATION, "Call to IcmpSendEcho failed. ErrorCode: %u", errorCode);
    }

    IcmpCloseHandle(hIcmpFile);
}

void DebugInfoNetworkWidget::WinAPIIcmpSendEcho2Test(const QString &destinationAddress)
{
    HANDLE hIcmpFile = INVALID_HANDLE_VALUE;
    IPAddr ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;
    DWORD errorCode = ERROR_SUCCESS;

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE)
    {
        errorCode = GetLastError();
        LogUtil::Error(CODE_LOCATION, "Unable to open handle. ErrorCode: %u", errorCode);
        return;
    }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData) + 8; // +8 ???
    ReplyBuffer = (void*)malloc(ReplySize);
    if (ReplyBuffer == NULL)
    {
        LogUtil::Error(CODE_LOCATION, "Unable to allocate memory");
        return;
    }

    ipaddr = inet_addr(destinationAddress.toStdString().c_str());
    dwRetVal = IcmpSendEcho2(hIcmpFile, NULL, NULL, NULL,
                             ipaddr, SendData, sizeof(SendData), NULL,
                             ReplyBuffer, ReplySize, 1000);
    if (dwRetVal != 0)
    {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;

        LogUtil::Info(CODE_LOCATION, "Sent icmp message to %s", destinationAddress.toStdString().c_str());
        LogUtil::Info(CODE_LOCATION, "Received %ld icmp message responses", dwRetVal);
        LogUtil::Info(CODE_LOCATION, "\t  Received from %s", inet_ntoa(ReplyAddr));
        LogUtil::Info(CODE_LOCATION, "\t  Roundtrip time = %lu milliseconds", pEchoReply->RoundTripTime);
        LogUtil::Info(CODE_LOCATION, "\t  Time to live = %u", pEchoReply->Options.Ttl);
        if (pEchoReply->Status == IP_SUCCESS)
        {
            LogUtil::Info(CODE_LOCATION, "\t  Status = %ld (IP_SUCCESS) %s", pEchoReply->Status, pEchoReply->Data);
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "\t  Failed with status code: %lu", pEchoReply->Status);
        }
    }
    else
    {
        errorCode = GetLastError();
        LogUtil::Error(CODE_LOCATION, "Call to IcmpSendEcho failed. ErrorCode: %u", errorCode);
    }
    if (ReplyBuffer != NULL)
    {
        free(ReplyBuffer);
    }
    IcmpCloseHandle(hIcmpFile);
}

void DebugInfoNetworkWidget::WinAPIIcmp6SendEcho2Test(const QString &destinationAddress)
{
    // IPv4 �� IPv6 ���Զ�Ӧ��ת������
    std::string ipString = destinationAddress.toStdString();
    IN_ADDR   ipv4Address = { 0 };
    IN6_ADDR  ipv6Address = { 0 };
    int iResult = inet_pton(AF_INET6, ipString.c_str(), &ipv6Address);
    if (iResult == 1) // ת���ɹ�
    {
        LogUtil::Info(CODE_LOCATION, "The input IPv6 ( %s ) convert successfully. %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                      ipString.c_str(),
                      ipv6Address.u.Word[0], ipv6Address.u.Word[1], ipv6Address.u.Word[2], ipv6Address.u.Word[3],
                      ipv6Address.u.Word[4], ipv6Address.u.Word[5], ipv6Address.u.Word[6], ipv6Address.u.Word[7]);
        char ipv6StringBuffer[46] = { 0 };
        const char* pIPv6String = inet_ntop(AF_INET6, &ipv6Address, ipv6StringBuffer, sizeof(ipv6StringBuffer));
        if (pIPv6String != NULL)
        {
            LogUtil::Info(CODE_LOCATION, "IPv6 : %s", pIPv6String);
        }
    }
    iResult = inet_pton(AF_INET, ipString.c_str(), &ipv4Address);
    if (iResult == 1) // ת���ɹ�
    {
        LogUtil::Info(CODE_LOCATION, "The input IPv4 ( %s ) convert successfully. 0x%08x", ipString.c_str(), ipv4Address.s_addr);
        char ipv4StringBuffer[16] = { 0 };
        const char* pIPv4String = inet_ntop(AF_INET, &ipv4Address, ipv4StringBuffer, sizeof(ipv4StringBuffer));
        if (pIPv4String != NULL)
        {
            LogUtil::Info(CODE_LOCATION, "IPv4 : %s", pIPv4String);
        }
    }

    return;

    HANDLE hIcmp6File = INVALID_HANDLE_VALUE;
    struct sockaddr_in6 ipv6Addr = { 0 };
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;
    DWORD errorCode = ERROR_SUCCESS;

    hIcmp6File = Icmp6CreateFile();
    if (hIcmp6File == INVALID_HANDLE_VALUE)
    {
        errorCode = GetLastError();
        LogUtil::Error(CODE_LOCATION, "Unable to open handle. ErrorCode: %u", errorCode);
        return;
    }

    ReplySize = sizeof(ICMPV6_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (void*)malloc(ReplySize);
    if (ReplyBuffer == NULL)
    {
        LogUtil::Error(CODE_LOCATION, "Unable to allocate memory");
        return;
    }

    //ipv6Addr = inet_addr(destinationAddress.toStdString().c_str());
    dwRetVal = Icmp6SendEcho2(hIcmp6File, NULL, NULL, NULL, NULL,
                              &ipv6Addr, SendData, sizeof(SendData),
                              NULL, ReplyBuffer, ReplySize, 1000);
    if (dwRetVal != 0)
    {
        PICMPV6_ECHO_REPLY pEchoReply = (PICMPV6_ECHO_REPLY)ReplyBuffer;
        IPV6_ADDRESS_EX replyAddr = pEchoReply->Address;

        LogUtil::Info(CODE_LOCATION, "Sent icmp message to %s", destinationAddress.toStdString().c_str());
        LogUtil::Info(CODE_LOCATION, "Received %ld icmp message responses", dwRetVal);
        //LogUtil::Info(CODE_LOCATION, "\t  Received from %s", inet_ntoa(replyAddr));
        //LogUtil::Info(CODE_LOCATION, "\t  Roundtrip time = %lu milliseconds", pEchoReply->RoundTripTime);
        //LogUtil::Info(CODE_LOCATION, "\t  Time to live = %u", pEchoReply->Options.Ttl);
        if (pEchoReply->Status == IP_SUCCESS)
        {
            LogUtil::Info(CODE_LOCATION, "\t  Status = %ld (IP_SUCCESS)", pEchoReply->Status);
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "\t  Failed with status code: %lu", pEchoReply->Status);
        }
    }
    else
    {
        errorCode = GetLastError();
        LogUtil::Error(CODE_LOCATION, "Call to IcmpSendEcho failed. ErrorCode: %u", errorCode);
    }

    IcmpCloseHandle(hIcmp6File);
}

ushort DebugInfoNetworkWidget::CaculateChecksum(uchar *InBuffer, uint BufferLen)
{
    uint sum = 0;
    ushort *tempBuffer;

    tempBuffer = (ushort *)InBuffer;

    while (BufferLen> 1)
    {
        sum += *tempBuffer++;
        BufferLen -= 2;
    }

    if (BufferLen)
    {
        sum += *(uchar *)tempBuffer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return ~sum;
}

void DebugInfoNetworkWidget::ParseHostInfo(struct hostent *pHostInfo)
{
    if (pHostInfo == NULL) return;

    int index = 0;
    struct in_addr hostAddress = { 0 };

    LogUtil::Debug(CODE_LOCATION, "Destination info:");
    LogUtil::Debug(CODE_LOCATION, "            name: %s", pHostInfo->h_name);
    //LogUtil::Debug(CODE_LOCATION, "         IP Addr: %s", inet_ntoa(*((struct in_addr*)pHostInfo->h_addr)));
    while (pHostInfo->h_addr_list[index] != 0)
    {
        hostAddress.s_addr = *(u_long *)pHostInfo->h_addr_list[index];
        LogUtil::Debug(CODE_LOCATION, "      IP Addr[%d]: %s", index, inet_ntoa(hostAddress));
        ++index;
    }
    LogUtil::Debug(CODE_LOCATION, "          Length: %d bytes", pHostInfo->h_length);
    //LogUtil::Debug(CODE_LOCATION, "            Type: %u", pHostInfo->h_addrtype);
    switch (pHostInfo->h_addrtype)
    {
    case AF_INET:
        LogUtil::Debug(CODE_LOCATION, "            Type: %s", "AF_INET");
        break;
    case AF_INET6:
        LogUtil::Debug(CODE_LOCATION, "            Type: %s", "AF_INET6");
        break;
    case AF_NETBIOS:
        LogUtil::Debug(CODE_LOCATION, "            Type: %s", "AF_NETBIOS");
        break;
    default:
        LogUtil::Debug(CODE_LOCATION, "            Type: %u", pHostInfo->h_addrtype);
        break;
    }
    char **pptr = NULL;
    index = 1;
    for (pptr = pHostInfo->h_aliases; *pptr != NULL; pptr++, index++)
    {
        LogUtil::Debug(CODE_LOCATION, "       Alias[%d]: %s", index, *pptr);
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
    int i = 0;
}

void DebugInfoNetworkWidget::on_btnPingTest_clicked()
{
    QString serverAddress = ui->leDestinationAddress->text();
    this->PingTest(serverAddress);
    this->WinAPIGetAddrInfoTest(serverAddress);
    this->WinAPIGetHostByAddrTest(serverAddress);
    this->WinAPIGetNameInfoTest(serverAddress, 80);
    this->WinAPIIcmpSendEchoTest(serverAddress);
    this->WinAPIIcmpSendEcho2Test(serverAddress);
    this->WinAPIIcmp6SendEcho2Test(serverAddress);
}
