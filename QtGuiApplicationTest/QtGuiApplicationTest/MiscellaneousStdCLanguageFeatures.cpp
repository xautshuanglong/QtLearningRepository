#include "MiscellaneousStdCLanguageFeatures.h"

#include <LogUtil.h>

MiscellaneousStdCLanguageFeatures::MiscellaneousStdCLanguageFeatures(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousStdCLanguageFeatures::~MiscellaneousStdCLanguageFeatures()
{
}

QString MiscellaneousStdCLanguageFeatures::GetTitle()
{
    return QObject::tr("C Language Features");
}

QString MiscellaneousStdCLanguageFeatures::GetTitleTooltip()
{
    return QObject::tr("Take advantage of the C language.");
}

MiscellaneousTestGroup MiscellaneousStdCLanguageFeatures::GetGroupID()
{
    return MiscellaneousTestGroup::Standard_Cpp;
}

MiscellaneousTestItem MiscellaneousStdCLanguageFeatures::GetItemID()
{
    return MiscellaneousTestItem::StdCpp_C_Language_Features;
}

/************************************************************************/
/* if 语句（单行多句，没有括号），只执行第一句。                           */
/************************************************************************/
void MiscellaneousStdCLanguageFeatures::on_btnIfCondition_clicked()
{
    int a = 0, b = 0;
    LogUtil::Debug(CODE_LOCATION, "a=%d b=%d", a, b);

    static bool condition = true;
    condition = !condition;

    if (condition)
    {
        a = 1;
        b = 1;
        LogUtil::Debug(CODE_LOCATION, "a=%d b=%d", a, b);
    }
    else
    {
        a = 2;
        b = 2;
        LogUtil::Debug(CODE_LOCATION, "a=%d b=%d", a, b);
    }

    if (condition) a = 3; b = 3;
    LogUtil::Debug(CODE_LOCATION, "a=%d b=%d", a, b);
    if (!condition) a = 4; b = 4;
    LogUtil::Debug(CODE_LOCATION, "a=%d b=%d", a, b);
}
