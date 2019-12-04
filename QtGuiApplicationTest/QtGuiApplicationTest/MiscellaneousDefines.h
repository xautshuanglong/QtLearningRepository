#pragma once

#include <QString>
#include <QList>
#include <QMetaType>

enum class MiscellaneousTestGroup
{
    WinAPI_Test,     // Windows 原生 API 测试
    Standard_Cpp,    // 标准 CPP 库函数测试
    QT_Test,         // QT 功能模块测试
    Database,        // 数据库相关测试
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestGroup)

enum class MiscellaneousTestItem
{
    WinAPI_Thread_beginthreadex,      // Windows API 线程操作
    StdCpp_Shared_Pointer,            // 标准库 共享指针
    QT_Printer_PDF,                   // QT 输出 PDF
    QT_Printer_Widget,                // QT 将 QWidget 输出到 PDF
    QT_Image_QImage,                  // QT 图像操作 QImage
    QT_Image_QPixmap,                 // QT 图像操作 QPixmap
    Database_Sqlite_wxSqlite3,        // 数据库操作 wxSqlite3
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};
extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];
