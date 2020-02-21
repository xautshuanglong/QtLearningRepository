#include "MiscellaneousZip.h"

#include <QFileDialog>

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

void MiscellaneousZip::on_btnFilesArchive_clicked()
{
    QString sourceDir = ui.leFilesArchive->text();
    QString targetFile = ui.leZipFilesSave->text();
}

void MiscellaneousZip::on_btnFilesArchiveOpen_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open files"), qApp->applicationDirPath(), "All (*);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)");
    ui.leFilesArchive->setText(files.join(";"));;
}

void MiscellaneousZip::on_btnFilesArchiveBrowse_clicked()
{
    QString zipFileSave = QFileDialog::getSaveFileName(this, tr("Save Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leZipFilesSave->setText(zipFileSave);
}

void MiscellaneousZip::on_btnFilesExtract_clicked()
{
    QString sourceFile = ui.leZipFilesOpen->text();
    QString targetDir = ui.leFilesExtract->text();
}

void MiscellaneousZip::on_btnFilesExtractOpen_clicked()
{
    QString zipFileOpen = QFileDialog::getOpenFileName(this, tr("Open Zip File"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leZipFilesOpen->setText(zipFileOpen);
}

void MiscellaneousZip::on_btnFilesExtractBrowse_clicked()
{
    QString targetDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leFilesExtract->setText(targetDir);
}

void MiscellaneousZip::on_btnDirectoryArchive_clicked()
{
    QString sourceDir = ui.leDirectoryArchive->text();
    QString targetFile = ui.leZipDirectorySave->text();
}

void MiscellaneousZip::on_btnDirectoryArchiveOpen_clicked()
{
    QString sourceDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leDirectoryArchive->setText(sourceDir);
}

void MiscellaneousZip::on_btnDirectoryArchiveBrowse_clicked()
{
    QString zipFileSave = QFileDialog::getSaveFileName(this, tr("Save Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leZipDirectorySave->setText(zipFileSave);
}

void MiscellaneousZip::on_btnDirectoryExtract_clicked()
{
    QString sourceFile = ui.leZipDirectoryOpen->text();
    QString targetDir = ui.leDirectoryExtract->text();
}

void MiscellaneousZip::on_btnDirectoryExtractOpen_clicked()
{
    QString zipFileOpen = QFileDialog::getOpenFileName(this, tr("Open Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leZipDirectoryOpen->setText(zipFileOpen);
}

void MiscellaneousZip::on_btnDirectoryExtractBrowse_clicked()
{
    QString extractDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leDirectoryExtract->setText(extractDir);
}
