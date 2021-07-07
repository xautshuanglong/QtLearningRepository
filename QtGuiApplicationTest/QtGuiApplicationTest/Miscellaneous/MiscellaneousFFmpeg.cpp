#include "MiscellaneousFFmpeg.h"
#include "ui_MiscellaneousFFmpeg.h"

#include "LogUtil.h"

MiscellaneousFFmpeg::MiscellaneousFFmpeg(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousFFmpeg())
{
    ui->setupUi(this);
}

MiscellaneousFFmpeg::~MiscellaneousFFmpeg()
{
    delete ui;
}

QString MiscellaneousFFmpeg::GetTitle()
{
    return QObject::tr("FFmpeg");
}

QString MiscellaneousFFmpeg::GetTitleTooltip()
{
    return QObject::tr("How to us ffmpeg library.");
}

MiscellaneousTestGroup MiscellaneousFFmpeg::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousFFmpeg::GetItemID()
{
    return MiscellaneousTestItem::Others_FFmpeg;
}

void MiscellaneousFFmpeg::on_btnAudioTest_clicked()
{
    int i = 0;
}

void MiscellaneousFFmpeg::on_btnVideoTest_clicked()
{
    int i = 0;
}

void MiscellaneousFFmpeg::on_btnPictureTest_clicked()
{
    int i = 0;
}

void MiscellaneousFFmpeg::on_btnEmptyTest_clicked()
{
    int i = 0;
}
