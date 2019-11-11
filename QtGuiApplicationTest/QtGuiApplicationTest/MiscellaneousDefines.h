#pragma once

#include <QString>

enum class MiscellaneousTestGroup
{
    WinAPI_Thread,
    QT_Printer
};

enum class MiscellaneousTestItem
{
    Thread_beginthreadex,
    Printer_PDF,
    Printer_Widget
};

struct MiscellaneousGroupTitle
{
    MiscellaneousTestGroup groupID;
    QString                groupTitle;
};

struct MiscellaneousItemTitle
{
    MiscellaneousTestItem itemID;
    QString               itemTitle;
};
