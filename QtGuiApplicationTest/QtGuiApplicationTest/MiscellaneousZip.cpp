#include "MiscellaneousZip.h"

MiscellaneousZip::MiscellaneousZip(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousZip::~MiscellaneousZip()
{
}

QString MiscellaneousZip::GetTitle()
{
    return QObject::tr("Zip");
}

QString MiscellaneousZip::GetTitleTooltip()
{
    return QObject::tr("How to use zip archive and extract files.");
}

MiscellaneousTestGroup MiscellaneousZip::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousZip::GetItemID()
{
    return MiscellaneousTestItem::Others_Zip;
}

void MiscellaneousZip::on_btnBrowseFiles_clicked()
{
    int i = 0;
}

void MiscellaneousZip::on_btnBrowseDirectory_clicked()
{
    int i = 0;
}

void MiscellaneousZip::on_btnFilesArchive_clicked()
{
    int i = 0;
}

void MiscellaneousZip::on_btnFilesExtract_clicked()
{
    int i = 0;
}

void MiscellaneousZip::on_btnDirectoryArchive_clicked()
{
    int i = 0;
}

void MiscellaneousZip::on_btnDirectoryExtract_clicked()
{
    int i = 0;
}
