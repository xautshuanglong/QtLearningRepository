#pragma once

#include <QString>
#include <QList>
#include <QMetaType>

enum class MiscellaneousTestGroup
{
    WinAPI_Test,     // Windows 原生 API 测试
    Standard_Cpp,    // 标准 CPP 库函数测试
    Algorithm,       // 算法练习
    QT_Test,         // QT 功能模块测试
    Database,        // 数据库相关测试
    Utility_Tools,   // 实用工具类测试入口
    Others,          // 其他未知分类
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestGroup)

enum class MiscellaneousTestItem
{
    WinAPI_Thread_BeginThreadEx,      // Windows API 线程操作
    WinAPI_Thread_WinPrintDlg,        // Windows API 打印机设置对话框
    StdCpp_Shared_Pointer,            // 标准库 共享指针
    StdCpp_Constructor,               // 标准库 构造函数：拷贝、移动、赋值
    StdCpp_C_Language_Features,       // C 语言特性测试
    Algorithm_LeetCode,               // LeetCode 力扣练习题
    QT_Signal_Slot,                   // QT 信号槽测试：传递自定义类型
    QT_Printer_PDF,                   // QT 输出 PDF
    QT_Printer_Widget,                // QT 将 QWidget 输出到 PDF
    QT_Image_QImage,                  // QT 图像操作 QImage
    QT_Image_QPixmap,                 // QT 图像操作 QPixmap
    QT_Object_Cast,                   // QT 跨越动态链接库 qobject_cast
    QT_Regular_Expression,            // QT 正则表达式训练
    QT_Mime_Type,                     // QT 文件类型检测 MIME（多用途互联网邮件扩展类型）
    QT_Excel_ReadWrite,               // QT Excel 文件读写
    QT_WEBSOCKET_CLIENT,              // QT QWebSocket 客户端测试
    QT_TCPSOCKET_CLIENT,              // QT QTcpSocket 客户端测试
    Database_Sqlite_wxSqlite3,        // 数据库操作 wxSqlite3
    Database_Sqlite_SqlCipher,        // 数据库操作 sqlcipher
    Database_Postgresql,              // 数据库操作 postgresql
    Utility_StringUtil,               // 工具类 - 字符串操作
    Others_Zip,                       // 使用 ZLib 归档/提取文件
    Others_ZLib,                      // 使用 ZLib 压缩/解压文件
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};
extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];
