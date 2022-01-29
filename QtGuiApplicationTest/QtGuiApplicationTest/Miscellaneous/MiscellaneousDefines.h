#pragma once

#include <QString>
#include <QList>
#include <QMetaType>

enum class MiscellaneousTestGroup
{
    WinAPI_Test,     // Windows ԭ�� API ����
    Standard_Cpp,    // ��׼ CPP �⺯������
    Algorithm,       // �㷨��ϰ
    QT_Test,         // QT ����ģ�����
    Database,        // ���ݿ���ز���
    Utility_Tools,   // ʵ�ù�����������
    Others,          // ����δ֪����
    Max_Size
};

Q_DECLARE_METATYPE(MiscellaneousTestGroup)

enum class MiscellaneousTestItem
{
    WinAPI_Thread_BeginThreadEx,      // Windows API �̲߳���
    WinAPI_Thread_WinPrintDlg,        // Windows API ��ӡ�����öԻ���
    WinAPI_Thread_WinSocket,          // Windows API socket ������
    WinAPI_EVent_Trace,               // ETW : Event Trace for Windows �¼�׷�ٻ��Ʋ���
    WinAPI_Registry,                  // ע�Ա���� �� ��������¼�
    WinAPI_WMI,                       // WMI ʹ�� �� �����¼�
    WinAPI_Hook,                      // ע��ϵͳ����
    StdCpp_Shared_Pointer,            // ��׼�� ����ָ��
    StdCpp_STL_Container,             // ��׼�� ����
    StdCpp_Constructor,               // ��׼�� ���캯�����������ƶ�����ֵ
    StdCpp_C_Language_Features,       // C �������Բ���
    Algorithm_LeetCode,               // LeetCode ������ϰ��
    QT_Signal_Slot,                   // QT �źŲ۲��ԣ������Զ�������
    QT_Printer_PDF,                   // QT ��� PDF
    QT_Printer_Widget,                // QT �� QWidget ����� PDF
    QT_Custom_TextEdit,               // QT �Զ����ı��༭��
    QT_Virtual_Keyboard,              // QT �Զ����ı��༭��
    QT_Image_QImage,                  // QT ͼ����� QImage
    QT_Image_QPixmap,                 // QT ͼ����� QPixmap
    QT_Image_Grabber,                 // QT ͼ����� Grab
    QT_Object_Cast,                   // QT ��Խ��̬���ӿ� qobject_cast
    QT_Timer,                         // QT Timer ��ʱ������
    QT_Regular_Expression,            // QT ������ʽѵ��
    QT_Mime_Type,                     // QT �ļ����ͼ�� MIME������;�������ʼ���չ���ͣ�
    QT_Excel_ReadWrite,               // QT Excel �ļ���д
    QT_WebSocket_Client,              // QT QWebSocket �ͻ��˲���
    QT_TcpSocket_Client,              // QT QTcpSocket �ͻ��˲���
    QT_Native_Window,                 // QT ԭ�����ڲ���
    Database_Sqlite_wxSqlite3,        // ���ݿ���� wxSqlite3
    Database_Sqlite_SqlCipher,        // ���ݿ���� SqlCipher
    Database_Postgresql,              // ���ݿ���� PostgreSQL
    Utility_StringUtil,               // ������ - �ַ�������
    Others_Zip,                       // ʹ�� ZLib �鵵/��ȡ�ļ�
    Others_ZLib,                      // ʹ�� ZLib ѹ��/��ѹ�ļ�
    Others_DirectShow,                // ʹ�� DirectShow ����Ӳ��ý���豸
    Others_Memory,                    // �ڴ�ṹ
    Others_FFmpeg,                    // ʹ�� FFmpeg ����Ƶ�ӿڲ���
    Others_TimeCode,                  // ʹ�� TimeCode ʱ����ѧϰ
    Max_Size
};

Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};
extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];
