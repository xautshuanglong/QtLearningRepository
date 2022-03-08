#include "MiscellaneousQTimerTest.h"
#include "ui_MiscellaneousQTimerTest.h"

#include <Windows.h>
#include <Mmddk.h>
#include <QTimer>

#include "Utils/TimeUtil.h"
#include "JCB_Logger/LogUtil.h"

using namespace Shuanglong::Utils;

MiscellaneousQTimerTest::MiscellaneousQTimerTest(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousQTimerTest())
    , mTimerEventID(0)
{
    ui->setupUi(this);
}

MiscellaneousQTimerTest::~MiscellaneousQTimerTest()
{
    delete ui;
}

QString MiscellaneousQTimerTest::GetTitle()
{
    return QObject::tr("QTimer Test");
}

QString MiscellaneousQTimerTest::GetTitleTooltip()
{
    return QObject::tr("How to us QTimer and learn timer algorithm.");
}

MiscellaneousTestGroup MiscellaneousQTimerTest::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQTimerTest::GetItemID()
{
    return MiscellaneousTestItem::QT_Timer;
}

void MiscellaneousQTimerTest::on_btnCustomTimer_clicked()
{
    int i = 0;
}

void MiscellaneousQTimerTest::on_btnDiffThread_clicked()
{
    int i = 0;
}

void MiscellaneousQTimerTest::on_btnTemporaryThread_clicked()
{
    int i = 0;
}

void MiscellaneousQTimerTest::on_btnSetCoalescableTimer_clicked()
{
    static int clickCount = 0;
    ++clickCount;
    if (clickCount % 2 == 0)
    {
        mTimerEventID = ::SetCoalescableTimer(nullptr, mTimerEventID, 10000, TimerProcdure, TIMERV_NO_COALESCING);
    }
    else
    {
        mTimerEventID = ::SetCoalescableTimer(nullptr, mTimerEventID, 5000, TimerProcdure, TIMERV_NO_COALESCING);
    }
}

void MiscellaneousQTimerTest::on_btnEmptyTest_clicked()
{
    int i = 0;
}

void MiscellaneousQTimerTest::TimerProcdure(HWND unnamedParam1, UINT unnamedParam2, UINT_PTR unnamedParam3, DWORD unnamedParam4)
{
    int i = 0;
}
