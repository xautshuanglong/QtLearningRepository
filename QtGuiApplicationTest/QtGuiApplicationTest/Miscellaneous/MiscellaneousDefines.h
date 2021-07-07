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
    QT_Object_Cast,                   // QT ��Խ��̬���ӿ� qobject_cast
    QT_Regular_Expression,            // QT �������ʽѵ��
    QT_Mime_Type,                     // QT �ļ����ͼ�� MIME������;�������ʼ���չ���ͣ�
    QT_Excel_ReadWrite,               // QT Excel �ļ���д
    QT_WebSocket_Client,              // QT QWebSocket �ͻ��˲���
    QT_TcpSocket_Client,              // QT QTcpSocket �ͻ��˲���
    Database_Sqlite_wxSqlite3,        // ���ݿ���� wxSqlite3
    Database_Sqlite_SqlCipher,        // ���ݿ���� sqlcipher
    Database_Postgresql,              // ���ݿ���� postgresql
    Utility_StringUtil,               // ������ - �ַ�������
    Others_Zip,                       // ʹ�� ZLib �鵵/��ȡ�ļ�
    Others_ZLib,                      // ʹ�� ZLib ѹ��/��ѹ�ļ�
    Others_DirectShow,                // ʹ�� DirectShow ����Ӳ��ý���豸
    Others_FFmpeg,                    // ʹ�� FFmpeg ����Ƶ�ӿڲ���
    Max_Size
};

Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};
extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];