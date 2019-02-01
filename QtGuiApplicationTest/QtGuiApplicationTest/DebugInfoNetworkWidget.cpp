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
//  12  |  0   |       |   *   |  IP header bad(catchall error)��������IP�ײ����������ֲ����
//  12  |  1   |       |   *   |  Required options missing����ȱ�ٱ����ѡ��
//  13  |  0   |   *   |       |  Timestamp request(obsolete)����ʱ����������ϲ��ã�
//  14  |      |   *   |       |  Timestamp reply(obsolete)����ʱ���Ӧ�����ϲ��ã�
//  15  |  0   |   *   |       |  Information request(obsolete)������Ϣ�������ϲ��ã�
//  16  |  0   |   *   |       |  Information reply(obsolete)������ϢӦ�����ϲ��ã�
//  17  |  0   |   *   |       |  Address mask request������ַ��������
//  18  |  0   |       |       |  Address mask reply������ַ����Ӧ��
//--------------------------------------------------------------------------------------------------------------------

// ICMP Type ����
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
//    17    |  Address Mask Request(�@ӍϢ���Á���ԃ�ӾW· mask �O����Ϣ)
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
//            ֵΪ 1 ʱ��������Сʱ�ӡ��������������߿ɿ��ԡ���С���á�
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

    //struct hostent *pHost = gethostbyname2()
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
            unsigned long timeDiff = GetTickCount() - pRecvICMP->OptinalData;
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
}