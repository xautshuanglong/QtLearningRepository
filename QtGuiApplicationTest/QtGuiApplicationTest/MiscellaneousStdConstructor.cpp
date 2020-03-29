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

    int i = 0;
}

void MiscellaneousStdConstructor::on_btnConstructorMove_clicked()
{
    int i = 0;
}

void MiscellaneousStdConstructor::on_btnConstructorCopy_clicked()
{
    int i = 0;
}

void MiscellaneousStdConstructor::on_btnConstructorAssign_clicked()
{
    int i = 0;
}

/*------------------------------------------------- ConstructorTestBase -------------------------------------------------*/
ConstructorTestBase::ConstructorTestBase()
    : mIntValue(0)
    , mStrName("NULL")
{
    LogUtil::Debug(CODE_LOCATION, "Construct Base Class");
}

ConstructorTestBase::ConstructorTestBase(const ConstructorTestBase& other)
{
    LogUtil::Debug(CODE_LOCATION, "Construct-Copy Base Class");
    mIntValue = other.mIntValue;
    mStrName = other.mStrName;
}

ConstructorTestBase::ConstructorTestBase(const ConstructorTestBase&& other)
{
    LogUtil::Debug(CODE_LOCATION, "Construct-Move Base Class");
    mIntValue = other.mIntValue;
    mStrName = other.mStrName;
}

ConstructorTestBase::~ConstructorTestBase()
{
    LogUtil::Debug(CODE_LOCATION, "Destruct Base Class");
}

ConstructorTestBase& ConstructorTestBase::operator=(const ConstructorTestBase& other)
{
    LogUtil::Debug(CODE_LOCATION, "Assign Base Class");
    if (this != &other)
    {
        mIntValue = other.mIntValue;
        mStrName = other.mStrName;
    }
    return *this;
}

void ConstructorTestBase::SetIntValue(const int intValue)
{
    mIntValue = intValue;
}

void ConstructorTestBase::SetName(const QString& name)
{
    mStrName = name;
}

int ConstructorTestBase::GetIntValue()
{
    return mIntValue;
}

QString ConstructorTestBase::GetName()
{
    return mStrName;
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
