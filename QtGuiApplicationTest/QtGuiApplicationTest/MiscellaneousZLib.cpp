#include "MiscellaneousZLib.h"

#include <QFileDialog>

MiscellaneousZLib::MiscellaneousZLib(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousZLib::~MiscellaneousZLib()
{
}

QString MiscellaneousZLib::GetTitle()
{
    return QObject::tr("ZLib");
}

QString MiscellaneousZLib::GetTitleTooltip()
{
    return QObject::tr("How to use zlib make archive files.");
}

MiscellaneousTestGroup MiscellaneousZLib::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousZLib::GetItemID()
{
    return MiscellaneousTestItem::Others_ZLib;
}

void MiscellaneousZLib::on_btnBrowseFiles_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open files"), qApp->applicationDirPath(), "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All(*)");
    ui.leFilenames->setText(files.join(";"));
}

void MiscellaneousZLib::on_btnBrowseDirectory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leDirectory->setText(dir);
}

void MiscellaneousZLib::on_btnArchiveFiles_clicked()
{
    int i = 0;
}

void MiscellaneousZLib::on_btnArchiveDirectory_clicked()
{
    int i = 0;
}
