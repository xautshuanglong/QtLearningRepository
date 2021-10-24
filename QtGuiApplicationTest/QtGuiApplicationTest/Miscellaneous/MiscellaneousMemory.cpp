#include "MiscellaneousMemory.h"
#include "ui_MiscellaneousMemory.h"

#include "JCB_Logger/LogUtil.h"

MiscellaneousMemory::MiscellaneousMemory(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousMemory())
{
    ui->setupUi(this);
}

MiscellaneousMemory::~MiscellaneousMemory()
{
    delete ui;
}

QString MiscellaneousMemory::GetTitle()
{
    return QObject::tr("Memory Header");
}

QString MiscellaneousMemory::GetTitleTooltip()
{
    return QObject::tr("Inspect memory malloc.");
}

MiscellaneousTestGroup MiscellaneousMemory::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousMemory::GetItemID()
{
    return MiscellaneousTestItem::Others_Memory;
}

void MiscellaneousMemory::on_btnMemoryHeaderTest_clicked()
{
    char* pCharTest = (char*)malloc(0xAAFE);

    int i = 0;
    free(pCharTest);
}

void MiscellaneousMemory::on_btnCaptureDeviceTest_clicked()
{
    int i = 0;
}

void MiscellaneousMemory::on_btnEmptyTest_1_clicked()
{
    int i = 0;
}

void MiscellaneousMemory::on_btnEmptyTest_2_clicked()
{
    int i = 0;
}
