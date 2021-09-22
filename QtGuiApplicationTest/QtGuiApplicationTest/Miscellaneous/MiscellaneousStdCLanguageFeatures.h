#ifndef MISCELLANEOUS_STD_C_LANGUAGE_FEATURES_H
#define MISCELLANEOUS_STD_C_LANGUAGE_FEATURES_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousStdCLanguageFeatures.h"

class MiscellaneousStdCLanguageFeatures : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousStdCLanguageFeatures(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousStdCLanguageFeatures();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnIfCondition_clicked();
    void on_btnVirtualInheritance_clicked();
    void on_btnCountIncrease_clicked();
    void on_btnStructBitField_clicked();

private:
    Ui::MiscellaneousStdCLanguageFeatures ui;
};

class IUnknownTest
{
public:
    IUnknownTest() {}
    virtual ~IUnknownTest() {}

    virtual void PureVirtualFunctionA() = 0;
    virtual void PureVirtualFunctionB() = 0;
};

class IUnknownDrivedTestA : public IUnknownTest
{
public:
    IUnknownDrivedTestA() {}
    virtual ~IUnknownDrivedTestA() {}

    virtual void PureVirtualFunctionDerivedA() = 0;
    virtual void PureVirtualFunctionA() override;
    virtual void PureVirtualFunctionB() override;
};

class IUnknownDrivedTestB : public IUnknownTest
{
public:
    IUnknownDrivedTestB() {}
    virtual ~IUnknownDrivedTestB() {}

    virtual void PureVirtualFunctionDerivedB() = 0;
    virtual void PureVirtualFunctionA() override;
    virtual void PureVirtualFunctionB() override;
};

class IUnknownImplementTest : public IUnknownDrivedTestA, public IUnknownDrivedTestB
{
public:
    IUnknownImplementTest();
    ~IUnknownImplementTest();

    virtual void PureVirtualFunctionA() override;
    virtual void PureVirtualFunctionB() override;
    virtual void PureVirtualFunctionDerivedA() override;
    virtual void PureVirtualFunctionDerivedB() override;
};

#endif // MISCELLANEOUS_STD_C_LANGUAGE_FEATURES_H
