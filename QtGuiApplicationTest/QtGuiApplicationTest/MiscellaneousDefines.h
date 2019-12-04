#pragma once

#include <QString>
#include <QList>
#include <QMetaType>

enum class MiscellaneousTestGroup
{
    WinAPI_Test,     // Windows ԭ�� API ����
    Standard_Cpp,    // ��׼ CPP �⺯������
    QT_Test,         // QT ����ģ�����
    Database,        // ���ݿ���ز���
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestGroup)

enum class MiscellaneousTestItem
{
    WinAPI_Thread_beginthreadex,      // Windows API �̲߳���
    StdCpp_Shared_Pointer,            // ��׼�� ����ָ��
    QT_Printer_PDF,                   // QT ��� PDF
    QT_Printer_Widget,                // QT �� QWidget ����� PDF
    QT_Image_QImage,                  // QT ͼ����� QImage
    QT_Image_QPixmap,                 // QT ͼ����� QPixmap
    Database_Sqlite_wxSqlite3,        // ���ݿ���� wxSqlite3
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};
extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];
