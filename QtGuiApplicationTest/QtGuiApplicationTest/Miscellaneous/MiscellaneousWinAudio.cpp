#include "MiscellaneousWinAudio.h"
#include "ui_MiscellaneousWinAudio.h"

#include <windows.h>
#include <tchar.h>

#include "JCB_Logger/LogUtil.h"

MiscellaneousWinAudio::MiscellaneousWinAudio(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousWinAudio())
    , mIsMonitorRegRunning(false)
    , mpRegMonitorThread(nullptr)
{
    ui->setupUi(this);
}

MiscellaneousWinAudio::~MiscellaneousWinAudio()
{
    delete ui;
}

QString MiscellaneousWinAudio::GetTitle()
{
    return QObject::tr("Audio");
}

QString MiscellaneousWinAudio::GetTitleTooltip()
{
    return QObject::tr("Using WAS API to play sound and monitor audio session event.");
}

MiscellaneousTestGroup MiscellaneousWinAudio::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinAudio::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Audio;
}

void MiscellaneousWinAudio::on_btnMonitorRegStart_clicked()
{
    int i = 0;
}

void MiscellaneousWinAudio::on_btnMonitorRegStop_clicked()
{
    int i = 0;
}

void MiscellaneousWinAudio::on_btnEventWrite_clicked()
{
    int i = 0;
}

void MiscellaneousWinAudio::on_btnEmptyTest_clicked()
{
    int i = 0;
}
