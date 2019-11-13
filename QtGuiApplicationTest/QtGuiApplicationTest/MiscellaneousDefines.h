#pragma once

#include <QString>
#include <QMetaType>

enum class MiscellaneousTestGroup
{
    WinAPI_Thread,
    QT_Printer,
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestGroup)

enum class MiscellaneousTestItem
{
    Thread_beginthreadex,
    Printer_PDF,
    Printer_Widget,
    Max_Size
};
Q_DECLARE_METATYPE(MiscellaneousTestItem)

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};

extern MiscellaneousGroupTitle gMiscellaneousGroupInfo[];
