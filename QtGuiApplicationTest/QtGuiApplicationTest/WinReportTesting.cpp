#include "WinReportTesting.h"

#include "ui_WinReportTesting.h"
#include "FramelessWindowHelper.h"

#include <MemUtil.h>

using namespace SL::Util;

WinReportTesting::WinReportTesting(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , ui(new Ui::WinReportTesting())
    , mpFramelessHelper(Q_NULLPTR)
{
    ui->setupUi(this);
    mpFramelessHelper = new FramelessWindowHelper(this);

    int *pA = 0;
    MemUtil::AquirePointer(&pA);
}

WinReportTesting::~WinReportTesting()
{
    MemUtil::RelesePointer(&mpFramelessHelper);
}
