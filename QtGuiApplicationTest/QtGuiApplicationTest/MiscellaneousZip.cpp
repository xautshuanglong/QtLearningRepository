#include "MiscellaneousZip.h"

#include <assert.h>
#include <QFileDialog>

#include <zip.h>
#include <unzip.h>
#include <libzip/zip.h>

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
    QStringList sourceFiles = ui.leFilesArchive->text().split(";");
    QString targetFile = ui.leZipFilesSave->text();

    assert(sourceFiles.count() != 0);
    assert(!targetFile.isEmpty());
    if (sourceFiles.count() == 0 || targetFile.isEmpty()) return;

    QFileInfo tempFileInfo;
    ZRESULT zres = ZR_OK;
    HZIP hZipFile;
    hZipFile = CreateZip(targetFile.toUtf8().data(), 0);

    for each (QString filename in sourceFiles)
    {
        tempFileInfo.setFile(filename);
        zres = ZipAdd(hZipFile, tempFileInfo.fileName().toUtf8().data(), filename.toUtf8().data());
        if (zres != ZR_OK)
        {
            break;
        }
    }

    CloseZip(hZipFile);
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

    assert(!sourceFile.isEmpty());
    assert(!targetDir.isEmpty());
    if (sourceFile.isEmpty() || targetDir.isEmpty()) return;

    ZRESULT zres = ZR_OK;
    HZIP hZipFile = OpenZip(sourceFile.toUtf8().data(), 0);
    zres = SetUnzipBaseDir(hZipFile, targetDir.toUtf8().data());
    if (zres == ZR_OK)
    {
        ZIPENTRY zipEntry;
        zres = GetZipItem(hZipFile, -1, &zipEntry);
        int numitems = zipEntry.index;
        if (zres == ZR_OK)
        {
            for (int zi = 0; zi < numitems; zi++)
            {
                zres = GetZipItem(hZipFile, zi, &zipEntry);
                zres = UnzipItem(hZipFile, zi, zipEntry.name);
            }
        }
    }
    CloseZip(hZipFile);
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

void MiscellaneousZip::on_btnLibZipArchive_clicked()
{
    int i = 0;

    QStringList sourceFiles = ui.leFilesArchive->text().split(";");
    QString targetFile = ui.leZipFilesSave->text();

    assert(sourceFiles.count() != 0);
    assert(!targetFile.isEmpty());
    if (sourceFiles.count() == 0 || targetFile.isEmpty()) return;
}

void MiscellaneousZip::on_btnLibZipArchiveOpen_clicked()
{
    int i = 0;

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open files"), qApp->applicationDirPath(), "All (*);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)");
    ui.leFilesArchive->setText(files.join(";"));;
}

void MiscellaneousZip::on_btnLibZipArchiveBrowse_clicked()
{
    int i = 0;

    QString zipFileSave = QFileDialog::getSaveFileName(this, tr("Save Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leZipFilesSave->setText(zipFileSave);
}

void MiscellaneousZip::on_btnLibZipExtract_clicked()
{
    int i = 0;

    QString sourceFile = ui.leZipFilesOpen->text();
    QString targetDir = ui.leFilesExtract->text();

    assert(!sourceFile.isEmpty());
    assert(!targetDir.isEmpty());
    if (sourceFile.isEmpty() || targetDir.isEmpty()) return;
}

void MiscellaneousZip::on_btnLibZipExtractOpen_clicked()
{
    int i = 0;

    QString zipFileOpen = QFileDialog::getOpenFileName(this, tr("Open Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leZipDirectoryOpen->setText(zipFileOpen);
}

void MiscellaneousZip::on_btnLibZipExtractBrowse_clicked()
{
    int i = 0;

    QString extractDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leDirectoryExtract->setText(extractDir);
}
