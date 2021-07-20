#ifndef MISCELLANEOUS_QTIMER_TEST_H
#define MISCELLANEOUS_QTIMER_TEST_H

#include <Windows.h>

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousQTimerTest;};

class QTimer;

class MiscellaneousQTimerTest : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQTimerTest(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQTimerTest();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnCustomTimer_clicked();
    void on_btnDiffThread_clicked();
    void on_btnTemporaryThread_clicked();
    void on_btnEmptyTest_clicked();

private:
    Ui::MiscellaneousQTimerTest *ui;
};

#endif // MISCELLANEOUS_QTIMER_TEST_H
