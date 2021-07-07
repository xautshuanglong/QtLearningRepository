#include "MiscellaneousStdConstructor.h"

#include <LogUtil.h>

MiscellaneousStdConstructor::MiscellaneousStdConstructor(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousStdConstructor::~MiscellaneousStdConstructor()
{
}

QString MiscellaneousStdConstructor::GetTitle()
{
    return QObject::tr("C++ Constructor");
}

QString MiscellaneousStdConstructor::GetTitleTooltip()
{
    return QObject::tr("How to us standard cpp constructor, copy assign move.");
}

MiscellaneousTestGroup MiscellaneousStdConstructor::GetGroupID()
{
    return MiscellaneousTestGroup::Standard_Cpp;
}

MiscellaneousTestItem MiscellaneousStdConstructor::GetItemID()
{
    return MiscellaneousTestItem::StdCpp_Constructor;
}

ConstructorTestBase MiscellaneousStdConstructor::GetLocalVariable()
{
    ConstructorTestBase localVariable;
    localVariable.SetIntValue(101);
    localVariable.SetName("local variable");
    std::string localVariableStr = localVariable.toString();
    LogUtil::Debug(CODE_LOCATION, "localVariable (0x%p): %s", &localVariable, localVariableStr.c_str());

    return localVariable;
}

void MiscellaneousStdConstructor::on_btnConstructorCommon_clicked()
{
    ConstructorTestBase testBase1;
    ConstructorTestDerived testDerived1;

    int baseIntValue = testBase1.GetIntValue();
    int derivedIntValue = testDerived1.GetIntValue();
    int baseValueInDerived = testDerived1.ConstructorTestBase::GetIntValue();

    testDerived1.ConstructorTestBase::SetIntValue(3);

    int baseIntValueNew = testBase1.GetIntValue();
    int derivedIntValueNew = testDerived1.GetIntValue();
    int baseValueInDerivedNew = testDerived1.ConstructorTestBase::GetIntValue();
}

void MiscellaneousStdConstructor::on_btnConstructorMove_clicked()
{
    ConstructorTestBase getLocalVar = this->GetLocalVariable();
    ConstructorTestBase test1(getLocalVar);  // 拷贝构造
    ConstructorTestBase test2 = getLocalVar; // 拷贝构造
    ConstructorTestBase test3 = test1;       // 拷贝构造

    ConstructorTestBase test4;
    test4 = test1; // 赋值操作符

    std::string getLocalVarStr = getLocalVar.toString();
    std::string test1Str = test1.toString();
    std::string test2Str = test1.toString();

    LogUtil::Debug(CODE_LOCATION, "getLocalVar (0x%p): %s", &getLocalVar, getLocalVarStr.c_str());
    LogUtil::Debug(CODE_LOCATION, "test1 (0x%p): %s", &test1, test1Str.c_str());
    LogUtil::Debug(CODE_LOCATION, "test2 (0x%p): %s", &test2, test2Str.c_str());

    ConstructorTestBase moveBefore;
    moveBefore.SetIntValue(111);
    moveBefore.SetName("moveTest");

    ConstructorTestBase moveAfter = std::move(moveBefore); // https://zhuanlan.zhihu.com/p/94588204

    std::string moveBeforeName = moveBefore.GetName();
    std::string moveAfterName = moveAfter.GetName();
}

void MiscellaneousStdConstructor::on_btnConstructorCopy_clicked()
{
    int i = 0;
}

void MiscellaneousStdConstructor::on_btnConstructorAssign_clicked()
{
    int i = 0;
}

void MiscellaneousStdConstructor::on_btnIncreasePre_clicked()
{
    ConstructorTestBase selfIncreaseTest;
    for (int i = 0; i < 10; ++i)
    {
        ++selfIncreaseTest;
    }
    int internalValue = selfIncreaseTest.GetIncreaseValue();
    LogUtil::Debug(CODE_LOCATION, "internalValue = %d", internalValue);
}

void MiscellaneousStdConstructor::on_btnIncreasePost_clicked()
{
    ConstructorTestBase selfIncreaseTest;
    for (int i = 0; i < 10; ++i)
    {
        selfIncreaseTest++;
    }
    int internalValue = selfIncreaseTest.GetIncreaseValue();
    LogUtil::Debug(CODE_LOCATION, "internalValue = %d", internalValue);
}

/*------------------------------------------------- ConstructorTestBase -------------------------------------------------*/
ConstructorTestBase::ConstructorTestBase() noexcept
    : mIntValue(0)
    , mSelfIncrease(0)
    , mStrName("NULL")
{
    LogUtil::Debug(CODE_LOCATION, "Construct Base Class Pointer:0x%p", this);
}

ConstructorTestBase::ConstructorTestBase(const ConstructorTestBase& other) noexcept
{
    LogUtil::Debug(CODE_LOCATION, "Construct-Copy Base Class ThisPointer:0x%p OtherPointer:0x%p", this, &other);
    mIntValue = other.mIntValue;
    mSelfIncrease = other.mSelfIncrease;
    mStrName = other.mStrName;
}

ConstructorTestBase::ConstructorTestBase(const ConstructorTestBase&& other) noexcept
{
    LogUtil::Debug(CODE_LOCATION, "Construct-Move Base Class ThisPointer:0x%p OtherPointer:0x%p", this, &other);
    mIntValue = other.mIntValue;
    mSelfIncrease = other.mSelfIncrease;
    mStrName = std::move(other.mStrName);
}

ConstructorTestBase::~ConstructorTestBase()
{
    LogUtil::Debug(CODE_LOCATION, "Destruct Base Class Pointer:0x%p", this);
}

ConstructorTestBase& ConstructorTestBase::operator=(const ConstructorTestBase& other)
{
    LogUtil::Debug(CODE_LOCATION, "Assign Base Class const Pointer:0x%p", this);
    if (this != &other)
    {
        mIntValue = other.mIntValue;
        mSelfIncrease = other.mSelfIncrease;
        mStrName = other.mStrName;
    }
    return *this;
}

ConstructorTestBase& ConstructorTestBase::operator=(ConstructorTestBase& other)
{
    LogUtil::Debug(CODE_LOCATION, "Assign Base Class non const Pointer:0x%p", this);
    if (this != &other)
    {
        mIntValue = other.mIntValue;
        mSelfIncrease = other.mSelfIncrease;
        mStrName = other.mStrName;
    }
    return *this;
}

/**
 * 前置 ++
 */
ConstructorTestBase& ConstructorTestBase::operator++()
{
    LogUtil::Debug(CODE_LOCATION, "ConstructorTestBase::operator++() before ThisPointer=0x%p  mSelfIncrease=%d", this, mSelfIncrease);
    LogUtil::Debug(CODE_LOCATION, "ConstructorTestBase::operator++() during ThisPointer=0x%p  mSelfIncrease=%d", this, ++mSelfIncrease);
    LogUtil::Debug(CODE_LOCATION, "ConstructorTestBase::operator++() after ThisPointer=0x%p  mSelfIncrease=%d", this, mSelfIncrease);

    return *this;
}

/**
 * 后置 ++
 */
ConstructorTestBase ConstructorTestBase::operator++(int)
{
    ConstructorTestBase retValue(*this);

    LogUtil::Debug(CODE_LOCATION, "ConstructorTestBase::operator++() before ThisPointer=0x%p  mSelfIncrease=%d", this, mSelfIncrease);
    LogUtil::Debug(CODE_LOCATION, "ConstructorTestBase::operator++() during ThisPointer=0x%p  mSelfIncrease=%d", this, mSelfIncrease++);
    LogUtil::Debug(CODE_LOCATION, "ConstructorTestBase::operator++() after ThisPointer=0x%p  mSelfIncrease=%d", this, mSelfIncrease);

    return retValue;
}

void ConstructorTestBase::SetIntValue(const int intValue)
{
    mIntValue = intValue;
}

void ConstructorTestBase::SetName(const std::string & name)
{
    mStrName = name;
}

int ConstructorTestBase::GetIntValue()
{
    return mIntValue;
}

int ConstructorTestBase::GetIncreaseValue()
{
    return mSelfIncrease;
}

std::string ConstructorTestBase::GetName()
{
    return mStrName;
}

std::string ConstructorTestBase::toString()
{
    std::string retValue;

    retValue += "name:";
    retValue += mStrName;
    retValue += " value:";
    retValue += std::to_string(mIntValue);

    return retValue;
}

/*------------------------------------------------- ConstructorTestDerived -------------------------------------------------*/
ConstructorTestDerived::ConstructorTestDerived()
    : mIntValue(1)
{
    LogUtil::Debug(CODE_LOCATION, "Construct Derived Class");
}

ConstructorTestDerived::~ConstructorTestDerived()
{
    LogUtil::Debug(CODE_LOCATION, "Destruct Derived Class");
}

void ConstructorTestDerived::SetIntValue(const int intValue)
{
    mIntValue = intValue;
}

int ConstructorTestDerived::GetIntValue()
{
    return mIntValue;
}
