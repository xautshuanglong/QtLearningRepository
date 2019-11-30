#pragma once

#include <QString>
#include <QList>
#include <QMetaType>

enum class MiscellaneousTestGroup
{
    WinAPI_Test,
    QT_Test,
    Database,
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestGroup)

enum class MiscellaneousTestItem
{
    WinAPI_Thread_beginthreadex,
    QT_Printer_PDF,
    QT_Printer_Widget,
    QT_Image_QImage,
    QT_Image_QPixmap,
    Database_Sqlite_wxSqlite3,
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};
extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];
