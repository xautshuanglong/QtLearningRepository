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
    WinAPI_Print_Dialog,              // Windows API 打印机设置对话框
    WinAPI_Socket,                    // Windows API socket 网络编程
    WinAPI_Event_Trace,               // ETW : Event Trace for Windows 事件追踪机制测试
    WinAPI_Registry,                  // 注冊表操作 和 监听变更事件
    WinAPI_WMI,                       // WMI 使用 和 监听事件
    WinAPI_Hook,                      // 注冊系统钩子
    WinAPI_Process,                   // 进程操作 和 基于进程的其他操作
    WinAPI_SystemInfo,                // 系统信息参数获取
    WinAPI_Audio,                     // 音频会话监控 与 音频流处理
    WinAPI_Bluetooth,                 // 操作蓝牙设备 与 蓝牙通信练习
    StdCpp_Shared_Pointer,            // 标准库 共享指针
    StdCpp_STL_Container,             // 标准库 容器
    StdCpp_Constructor,               // 标准库 构造函数：拷贝、移动、赋值
    StdCpp_C_Language_Features,       // C 语言特性测试
    Algorithm_LeetCode,               // LeetCode 力扣练习题
    QT_Signal_Slot,                   // QT 信号槽测试：传递自定义类型
    QT_Printer_PDF,                   // QT 输出 PDF
    QT_Printer_Widget,                // QT 将 QWidget 输出到 PDF
    QT_Custom_TextEdit,               // QT 自定义文本编辑框
    QT_Virtual_Keyboard,              // QT 自定义文本编辑框
    QT_Image_QImage,                  // QT 图像操作 QImage
    QT_Image_QPixmap,                 // QT 图像操作 QPixmap
    QT_Image_Grabber,                 // QT 图像操作 Grab
    QT_Object_Cast,                   // QT 跨越动态链接库 qobject_cast
    QT_Timer,                         // QT Timer 定时器测试
    QT_Regular_Expression,            // QT 正则表达式训练
    QT_Mime_Type,                     // QT 文件类型检测 MIME（多用途互联网邮件扩展类型）
    QT_Excel_ReadWrite,               // QT Excel 文件读写
    QT_WebSocket_Client,              // QT QWebSocket 客户端测试
    QT_TcpSocket_Client,              // QT QTcpSocket 客户端测试
    QT_Native_Window,                 // QT 原生窗口测试
    Database_Sqlite_wxSqlite3,        // 数据库操作 wxSqlite3
    Database_Sqlite_SqlCipher,        // 数据库操作 SqlCipher
    Database_Postgresql,              // 数据库操作 PostgreSQL
    Utility_StringUtil,               // 工具类 - 字符串操作
    Others_Zip,                       // 使用 ZLib 归档/提取文件
    Others_ZLib,                      // 使用 ZLib 压缩/解压文件
    Others_DirectShow,                // 使用 DirectShow 处理硬件媒体设备
    Others_Memory,                    // 内存结构
    Others_FFmpeg,                    // 使用 FFmpeg 音视频接口测试
    Others_TimeCode,                  // 使用 TimeCode 时间码学习
    Max_Size
};

Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};
extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];
