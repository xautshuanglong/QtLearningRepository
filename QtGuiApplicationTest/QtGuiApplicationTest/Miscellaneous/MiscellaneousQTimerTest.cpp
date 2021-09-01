#include "MiscellaneousQTimerTest.h"
#include "ui_MiscellaneousQTimerTest.h"

#include <Windows.h>
#include <Mmddk.h>
#include <QTimer>

#include "Utils/TimeUtil.h"
#include "LogUtil.h"

using namespace Shuanglong::Utils;

MiscellaneousQTimerTest::MiscellaneousQTimerTest(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousQTimerTest())
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

void MiscellaneousQTimerTest::on_btnEmptyTest_clicked()
{
    int i = 0;
}
