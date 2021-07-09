#include "MiscellaneousTimeCode.h"
#include "ui_MiscellaneousTimeCode.h"

#include <Windows.h>

#include <QTimer>

#include "Utils/TimeUtil.h"
#include "LogUtil.h"

using namespace Shuanglong::Utils;

MiscellaneousTimeCode::MiscellaneousTimeCode(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousTimeCode())
    , mbTimeCodeEnable(false)
{
    ui->setupUi(this);

    // UI 控件初始化
    mBtnTimeEmiterText = ui->btnTimeEmiterTest->text();
    ui->btnTimeEmiterTest->setText(QString("%1 (disable)").arg(mBtnTimeEmiterText));

    // 时间码触发器
    mpTimeCodeEmiter = new QTimer(this);
    mpTimeCodeEmiter->setInterval(std::chrono::milliseconds(1));
    connect(mpTimeCodeEmiter, SIGNAL(timeout()), this, SLOT(TimeCodeEmiter_TimeOut()));

    // 高性能计数器
    mCurFrequency = TimeUtil::QueryPerformanceFrequency();
}

MiscellaneousTimeCode::~MiscellaneousTimeCode()
{
    delete ui;
}

QString MiscellaneousTimeCode::GetTitle()
{
    return QObject::tr("Time Code");
}

QString MiscellaneousTimeCode::GetTitleTooltip()
{
    return QObject::tr("How to us time code MTC/LTC.");
}

MiscellaneousTestGroup MiscellaneousTimeCode::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousTimeCode::GetItemID()
{
    return MiscellaneousTestItem::Others_TimeCode;
}

void MiscellaneousTimeCode::TimeCodeEmiter_TimeOut()
{
    //static LONGLONG oldTickCount = TimeUtil::QueryPerformanceCounter();
    //LONGLONG curTickCount = TimeUtil::QueryPerformanceCounter();
    //LONGLONG diffCount = curTickCount - oldTickCount;
    //oldTickCount = curTickCount;
    //double microSecond = diffCount * 1000000 / mCurFrequency;
    //LogUtil::Debug(CODE_LOCATION, "curFrequency: %lld    curTickCount: %lld    diff: %lld  ( %llf us )",
    //    mCurFrequency, curTickCount, diffCount, microSecond);

    // 60fpx    --> 166667 = 16.6667ms * 10^4 = 0.0166667s * 10^7
    // 30fpx    --> 333333 = 33.3333ms * 10^4 = 0.0333333s * 10^7
    // 29.97fpx --> 333667 = 33.3667ms * 10^4 = 0.0333667s * 10^7
    // 25fpx    --> 400000 = 40.0000ms * 10^4 = 0.0400000s * 10^7
    // 24fpx    --> 416667 = 41.6667ms * 10^4 = 0.0416667s * 10^7

    static LONGLONG oldTickCount = TimeUtil::QueryPerformanceCounter();
    LONGLONG curTickCount = TimeUtil::QueryPerformanceCounter();
    LONGLONG diffTickCount = curTickCount - oldTickCount;

    bool emitFlag = false;
    if (diffTickCount >= 166666)
    {
        emitFlag = true; // 超时触发
    }
    else if (diffTickCount > 160000)
    {
        while (diffTickCount < 166666)
        {
            curTickCount = TimeUtil::QueryPerformanceCounter();
            diffTickCount = curTickCount - oldTickCount;
        }
        emitFlag = true;
    }
    if (emitFlag)
    {
        oldTickCount = curTickCount;
        double microSecond = diffTickCount * 1000.0 / mCurFrequency;
        LogUtil::Debug(CODE_LOCATION, "curFrequency: %lld    diffTickCount: %lld    %llf ms",
            mCurFrequency, diffTickCount, microSecond);
    }
}

void MiscellaneousTimeCode::on_btnTransferTest_clicked()
{
    int i = 0;
}

void MiscellaneousTimeCode::on_btnEventMapTest_clicked()
{
    int i = 0;
}

void MiscellaneousTimeCode::on_btnTimeEmiterTest_clicked()
{
    if (mbTimeCodeEnable)
    {
        mpTimeCodeEmiter->stop();
    }
    else
    {
        mpTimeCodeEmiter->start();
    }
    mbTimeCodeEnable = !mbTimeCodeEnable;
    if (mbTimeCodeEnable)
    {
        ui->btnTimeEmiterTest->setText(QString("%1 (enable)").arg(mBtnTimeEmiterText));
    } 
    else
    {
        ui->btnTimeEmiterTest->setText(QString("%1 (disable)").arg(mBtnTimeEmiterText));
    }
}

void MiscellaneousTimeCode::on_btnEmptyTest_2_clicked()
{
    int i = 0;
}
