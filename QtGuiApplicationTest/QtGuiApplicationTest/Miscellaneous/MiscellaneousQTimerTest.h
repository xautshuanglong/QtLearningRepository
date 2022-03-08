#ifndef MISCELLANEOUS_QTIMER_TEST_H
#define MISCELLANEOUS_QTIMER_TEST_H

#include "MiscellaneousBase.h"

#include <Windows.h>

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

    static void TimerProcdure(HWND unnamedParam1, UINT unnamedParam2, UINT_PTR unnamedParam3, DWORD unnamedParam4);

private slots:
    void on_btnCustomTimer_clicked();
    void on_btnDiffThread_clicked();
    void on_btnTemporaryThread_clicked();
    void on_btnSetCoalescableTimer_clicked();
    void on_btnEmptyTest_clicked();

private:
    Ui::MiscellaneousQTimerTest *ui;
    UINT_PTR                     mTimerEventID;
};

#endif // MISCELLANEOUS_QTIMER_TEST_H
