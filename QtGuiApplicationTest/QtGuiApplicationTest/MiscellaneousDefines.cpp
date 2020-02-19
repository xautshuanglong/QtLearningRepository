#include "MiscellaneousDefines.h"

#include <QObject>

MiscellaneousGroupTitle gMiscellaneousGroupInfo[] =
{
    { MiscellaneousTestGroup::WinAPI_Test, QObject::tr("WinAPI Testing") },
    { MiscellaneousTestGroup::Standard_Cpp, QObject::tr("Standard CPP") },
    { MiscellaneousTestGroup::QT_Test, QObject::tr("QT Testing") },
    { MiscellaneousTestGroup::Database, QObject::tr("Database") },
    { MiscellaneousTestGroup::Others, QObject::tr("Others") }
};
