#include "MiscellaneousStdCLanguageFeatures.h"

#include "JCB_Logger/LogUtil.h"

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

void MiscellaneousStdCLanguageFeatures::on_btnCountIncrease_clicked()
{
    int count = 0;
    for (int i = 0; i < 100; ++i)
    {
        count = count++;
    }

    // 循环结束时 count == 0
    /*
    * 00007FF60AED3DC5  sub         rsp,18h
    int count = 0;
00007FF60AED3DC9  mov         dword ptr [rsp],0
    for (int i = 0; i < 100; ++i)
00007FF60AED3DD0  mov         dword ptr [rsp+4],0
00007FF60AED3DD8  jmp         IUnknownDrivedTestA::PureVirtualFunctionA+4h (07FF60AED3DE4h)
00007FF60AED3DDA  mov         eax,dword ptr [rsp+4]
00007FF60AED3DDE  inc         eax
00007FF60AED3DE0  mov         dword ptr [rsp+4],eax
00007FF60AED3DE4  cmp         dword ptr [rsp+4],64h
00007FF60AED3DE9  jge         IUnknownDrivedTestA::PureVirtualFunctionA+23h (07FF60AED3E03h)
    {
        count = count++;
00007FF60AED3DEB  mov         eax,dword ptr [rsp]
00007FF60AED3DEE  mov         dword ptr [rsp+8],eax
00007FF60AED3DF2  mov         eax,dword ptr [rsp]
00007FF60AED3DF5  inc         eax
00007FF60AED3DF7  mov         dword ptr [rsp],eax
00007FF60AED3DFA  mov         eax,dword ptr [rsp+8]
00007FF60AED3DFE  mov         dword ptr [rsp],eax
    }
    */
}

void MiscellaneousStdCLanguageFeatures::on_btnStructBitField_clicked()
{
    struct S
    {
        // will usually occupy 2 bytes:
        // 3 bits: value of b1
        // 5 bits: unused
        // 6 bits: value of b2
        // 2 bits: value of b3
        unsigned char b1 : 3;
        unsigned char : 0; // start a new byte
        unsigned char b2 : 6;
        unsigned char b3 : 2;
    };
    int size1 = sizeof(S);

    int a;
    const int b = 0;
    struct S2
    {
        // simple cases
        int x1 : 8 = 42;                 // OK; "= 42" is brace-or-equal-initializer
        int x2 : 8 { 42 };               // OK; "{ 42 }" is brace-or-equal-initializer
        // ambiguities
        int y1 : true ? 8 : a = 42;      // OK; brace-or-equal-initializer is absent
        //int y2 : true ? 8 : b = 42;      // error: cannot assign to const int
        //int y2 : true ? 8 : 42;      // error: cannot assign to const int
        int y3 : (true ? 8 : b) = 42;    // OK; "= 42" is brace-or-equal-initializer
        int z : 1 || new int{ 0 };       // OK; brace-or-equal-initializer is absent
    };
    int size2 = sizeof(S2);

    int i = 0;
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
