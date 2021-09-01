#include "MiscellaneousQNativeWindow.h"
#include "ui_MiscellaneousQNativeWindow.h"

#include "Utils/TimeUtil.h"
#include "LogUtil.h"

using namespace Shuanglong::Utils;

MiscellaneousQNativeWindow::MiscellaneousQNativeWindow(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousQNativeWindow())
{
    ui->setupUi(this);
}

MiscellaneousQNativeWindow::~MiscellaneousQNativeWindow()
{
    delete ui;
}

QString MiscellaneousQNativeWindow::GetTitle()
{
    return QObject::tr("Native Window Test");
}

QString MiscellaneousQNativeWindow::GetTitleTooltip()
{
    return QObject::tr("How to embed a native window in QT application.");
}

MiscellaneousTestGroup MiscellaneousQNativeWindow::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQNativeWindow::GetItemID()
{
    return MiscellaneousTestItem::QT_Native_Window;
}

void MiscellaneousQNativeWindow::on_btnCustomTimer_clicked()
{
    int i = 0;
}

void MiscellaneousQNativeWindow::on_btnDiffThread_clicked()
{
    int i = 0;
}

void MiscellaneousQNativeWindow::on_btnTemporaryThread_clicked()
{
    int i = 0;
}

void MiscellaneousQNativeWindow::on_btnEmptyTest_clicked()
{
    int i = 0;
}
