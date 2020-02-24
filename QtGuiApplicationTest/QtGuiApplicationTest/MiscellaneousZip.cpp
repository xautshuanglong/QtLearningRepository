#include "MiscellaneousZip.h"

#include <assert.h>
#include <QFileDialog>

#include <zip.h>
#include <unzip.h>
#include <libzip/zip.h>

#include <LogUtil.h>

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

    QStringList sourceFiles = ui.leLibZipArchive->text().split(";");
    QString targetFile = ui.leLibZipSave->text();

    assert(sourceFiles.count() != 0);
    assert(!targetFile.isEmpty());
    if (sourceFiles.count() == 0 || targetFile.isEmpty()) return;
}

void MiscellaneousZip::on_btnLibZipArchiveOpen_clicked()
{
    int i = 0;

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open files"), qApp->applicationDirPath(), "All (*);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)");
    ui.leLibZipArchive->setText(files.join(";"));;
}

void MiscellaneousZip::on_btnLibZipArchiveBrowse_clicked()
{
    int i = 0;

    QString zipFileSave = QFileDialog::getSaveFileName(this, tr("Save Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leLibZipSave->setText(zipFileSave);
}

void MiscellaneousZip::on_btnLibZipExtract_clicked()
{
    int i = 0;

    QString sourceFile = ui.leLibZipOpen->text();
    QString targetDir = ui.leLibZipExtract->text();

    assert(!sourceFile.isEmpty());
    assert(!targetDir.isEmpty());
    if (sourceFile.isEmpty() || targetDir.isEmpty()) return;

    int errCode = ZIP_ER_OK;
    zip_t *pZipArchive = NULL;
    pZipArchive = zip_open(sourceFile.toUtf8().data(), ZIP_RDONLY, &errCode);
    if (pZipArchive == NULL)
    {
        zip_error_t error;
        zip_error_init_with_code(&error, errCode);
        LogUtil::Error(CODE_LOCATION, "Open file %s failed. Error[%d]: %s", sourceFile.toUtf8().data(), errCode, zip_error_strerror(&error));
        zip_error_fini(&error);
        return;
    }

    zip_int64_t index;
    zip_int64_t entryCount = zip_get_num_entries(pZipArchive, 0);
    if (entryCount < 0)
    {
        LogUtil::Error(CODE_LOCATION, "Can not get number of entries for %s. Error: %s", sourceFile.toUtf8().data(), zip_strerror(pZipArchive));
        zip_close(pZipArchive);
        return;
    }

    char readBuffer[8192] = { 0 };
    const char *pFilenameInZip = NULL;
    zip_int64_t readCount = 0;
    zip_error_t error_got, error_ex;
    zip_error_t *pZipFileError = NULL;
    zip_file_t *pZipFileItem = NULL;
    zip_stat_t fileStatInZip;

    zip_stat_init(&fileStatInZip);
    zip_error_init(&error_got);
    zip_error_init(&error_ex);
    zip_error_set(&error_ex, ZIP_ER_OK, 0);

    for (int i = 0; i < entryCount; ++i)
    {
        pFilenameInZip = zip_get_name(pZipArchive, i, ZIP_FL_ENC_GUESS);
        pZipFileItem = zip_fopen(pZipArchive, pFilenameInZip, ZIP_FL_NODIR);
        if (pZipFileItem == NULL)
        {
            pZipFileError = zip_get_error(pZipArchive);
            zip_error_set(&error_got, zip_error_code_zip(pZipFileError), zip_error_code_system(pZipFileError));
        }
        else
        {
            QString outFilename = targetDir + "/" + pFilenameInZip;
            FILE *pExtractFile = fopen(outFilename.toUtf8().data(), "wb");
            int readTime = 0;
            while ((readCount = zip_fread(pZipFileItem, readBuffer, sizeof(readBuffer))) > 0)
            {
                ++readTime;
                LogUtil::Debug(CODE_LOCATION, "read time %02d       read count: %04d", readTime, readCount);
                for (int i = 0; i<readCount; ++i)
                {
                    fputc(readBuffer[i], pExtractFile);
                }
            }
            if (pExtractFile != NULL)
            {
                fclose(pExtractFile);
            }
            if (readCount < 0)
            {
                pZipFileError = zip_file_get_error(pZipFileItem);
                zip_error_set(&error_got, zip_error_code_zip(pZipFileError), zip_error_code_system(pZipFileError));
            }
        }
        index = zip_name_locate(pZipArchive, pFilenameInZip, ZIP_FL_NODIR);
        if (index >= 0)
        {
            zip_stat(pZipArchive, pFilenameInZip, 0, &fileStatInZip);
            LogUtil::Info(CODE_LOCATION, "%5d --> %s [%s] size:%llu time:%lld CRC:%08X", index, pFilenameInZip,
                          fileStatInZip.name, fileStatInZip.size, fileStatInZip.mtime, fileStatInZip.crc);
        }
        else
        {
            LogUtil::Info(CODE_LOCATION, "-----------------------------");
        }
    }

    zip_close(pZipArchive);
    zip_error_fini(&error_got);
    zip_error_fini(&error_ex);
}

void MiscellaneousZip::on_btnLibZipExtractOpen_clicked()
{
    int i = 0;

    QString zipFileOpen = QFileDialog::getOpenFileName(this, tr("Open Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leLibZipOpen->setText(zipFileOpen);
}

void MiscellaneousZip::on_btnLibZipExtractBrowse_clicked()
{
    int i = 0;

    QString extractDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leLibZipExtract->setText(extractDir);
}
