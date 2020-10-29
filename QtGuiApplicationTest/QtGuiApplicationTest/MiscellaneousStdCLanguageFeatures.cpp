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

void MiscellaneousStdCLanguageFeatures::on_btnVirtualInheritance_clicked()
{
    IUnknownImplementTest implTest;
    implTest.PureVirtualFunctionA();
    implTest.PureVirtualFunctionB();
    implTest.PureVirtualFunctionDerivedA();
    implTest.PureVirtualFunctionDerivedB();

    implTest.IUnknownDrivedTestA::PureVirtualFunctionA();
    implTest.IUnknownDrivedTestA::PureVirtualFunctionB();

    implTest.IUnknownDrivedTestB::PureVirtualFunctionA();
    implTest.IUnknownDrivedTestB::PureVirtualFunctionB();

    implTest.IUnknownImplementTest::PureVirtualFunctionA();
    implTest.IUnknownImplementTest::PureVirtualFunctionB();

    IUnknownTest* pUnknownTestA = (IUnknownDrivedTestA*)&implTest;
    pUnknownTestA->PureVirtualFunctionA();
    pUnknownTestA->PureVirtualFunctionB();

    IUnknownTest* pUnknownTestB = (IUnknownDrivedTestB*)&implTest;
    pUnknownTestB->PureVirtualFunctionA();
    pUnknownTestB->PureVirtualFunctionB();
}

IUnknownImplementTest::IUnknownImplementTest()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownImplementTest Construct ...");
}

IUnknownImplementTest::~IUnknownImplementTest()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownImplementTest Destruct ...");
}

void IUnknownImplementTest::PureVirtualFunctionA()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownImplementTest::PureVirtualFunctionA ...");
}

void IUnknownImplementTest::PureVirtualFunctionB()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownImplementTest::PureVirtualFunctionB ...");
}

void IUnknownImplementTest::PureVirtualFunctionDerivedA()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownImplementTest::PureVirtualFunctionDerivedA ...");
}

void IUnknownImplementTest::PureVirtualFunctionDerivedB()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownImplementTest::PureVirtualFunctionDerivedB ...");
}

void IUnknownDrivedTestA::PureVirtualFunctionA()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownDrivedTestA::PureVirtualFunctionA ...");
}

void IUnknownDrivedTestA::PureVirtualFunctionB()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownDrivedTestA::PureVirtualFunctionB ...");
}

void IUnknownDrivedTestB::PureVirtualFunctionA()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownDrivedTestB::PureVirtualFunctionA ...");
}

void IUnknownDrivedTestB::PureVirtualFunctionB()
{
    LogUtil::Debug(CODE_LOCATION, "Inside Func IUnknownDrivedTestB::PureVirtualFunctionB ...");
}
