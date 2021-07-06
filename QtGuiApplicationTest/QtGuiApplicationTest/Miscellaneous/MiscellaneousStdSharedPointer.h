#ifndef MISCELLANEOUS_STD_SHARED_POINTER_H
#define MISCELLANEOUS_STD_SHARED_POINTER_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousStdSharedPointer;};

class MiscellaneousStdSharedPointer : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousStdSharedPointer(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousStdSharedPointer();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnResetTest_clicked();
    void on_btnRefCountTest_clicked();

private:
    Ui::MiscellaneousStdSharedPointer *ui;
};

class AutoDestuctTest
{
public:
    AutoDestuctTest(const std::string& name, int value);
    ~AutoDestuctTest();

private:
    std::string   mName;
    int           mValue;
};

#endif // MISCELLANEOUS_STD_SHARED_POINTER_H
