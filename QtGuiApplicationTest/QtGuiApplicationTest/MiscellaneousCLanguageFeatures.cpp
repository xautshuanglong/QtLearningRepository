#include "MiscellaneousCLanguageFeatures.h"

#include <LogUtil.h>

MiscellaneousCLanguageFeatures::MiscellaneousCLanguageFeatures(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousCLanguageFeatures::~MiscellaneousCLanguageFeatures()
{
}

QString MiscellaneousCLanguageFeatures::GetTitle()
{
    return QObject::tr("C Language Features");
}

QString MiscellaneousCLanguageFeatures::GetTitleTooltip()
{
    return QObject::tr("Take advantage of the C language.");
}

MiscellaneousTestGroup MiscellaneousCLanguageFeatures::GetGroupID()
{
    return MiscellaneousTestGroup::Standard_Cpp;
}

MiscellaneousTestItem MiscellaneousCLanguageFeatures::GetItemID()
{
    return MiscellaneousTestItem::StdCpp_C_Language_Features;
}

void MiscellaneousCLanguageFeatures::on_btnIfCondition_clicked()
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
