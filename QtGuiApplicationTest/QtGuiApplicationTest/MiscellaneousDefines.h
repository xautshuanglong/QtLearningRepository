#pragma once

#include <QString>
#include <QMetaType>

enum class MiscellaneousTestGroup
{
    WinAPI_Thread,
    QT_Printer,
    Database,
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestGroup)

enum class MiscellaneousTestItem
{
    Thread_beginthreadex,
    Printer_PDF,
    Printer_Widget,
    Sqlite_wxSqlite3,
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};

extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];
