#ifndef MISCELLANEOUS_STD_CONSTRUCTOR_H
#define MISCELLANEOUS_STD_CONSTRUCTOR_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousStdConstructor.h"

class MiscellaneousStdConstructor : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousStdConstructor(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousStdConstructor();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnConstructorCommon_clicked();
    void on_btnConstructorMove_clicked();
    void on_btnConstructorCopy_clicked();
    void on_btnConstructorAssign_clicked();
    void on_btnIncreasePre_clicked();
    void on_btnIncreasePost_clicked();

private:
    Ui::MiscellaneousStdConstructor ui;
};

class ConstructorTestBase
{
public:
    ConstructorTestBase() noexcept;
    ConstructorTestBase(const ConstructorTestBase& other) noexcept;
    ConstructorTestBase(const ConstructorTestBase&& other) noexcept;
    ~ConstructorTestBase();
    ConstructorTestBase& operator=(const ConstructorTestBase& other);
    ConstructorTestBase& operator=(ConstructorTestBase& other);
    ConstructorTestBase& operator++();
    ConstructorTestBase operator++(int);

    void SetIntValue(const int intValue);
    void SetName(const QString& name);

    int GetIntValue();
    int GetIncreaseValue();
    QString GetName();

private:
    int       mIntValue;
    int       mSelfIncrease;
    QString   mStrName;
};

class ConstructorTestDerived : public ConstructorTestBase
{
public:
    ConstructorTestDerived();
    ~ConstructorTestDerived();

    void SetIntValue(const int intValue);
    int GetIntValue();

private:
    int   mIntValue;
};

#endif // MISCELLANEOUS_STD_CONSTRUCTOR_H
