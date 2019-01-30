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

// ICMP Э�������

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
