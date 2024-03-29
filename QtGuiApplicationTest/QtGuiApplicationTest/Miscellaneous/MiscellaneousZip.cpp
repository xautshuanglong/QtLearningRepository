#include "MiscellaneousZip.h"

#include <assert.h>
#include <QFileDialog>
#include <QMenu>
#include <QStack>

#include <sys/stat.h>
#include <sys/utime.h>
#include <direct.h>

#include <zip.h>
#include <unzip.h>
#include <libzip/zip.h>

#include "JCB_Logger/LogUtil.h"

MiscellaneousZip::MiscellaneousZip(QWidget *parent)
    : MiscellaneousBase(parent)
    , m_libZipArchiveType(MiscellaneousZip::ELZAOT_UNKNOWN)
{
    ui.setupUi(this);

    // 托盘右键菜单栏
    QMenu *pMenuLibZipArchiveOpen = new QMenu(this);
    QAction *pActionArchiveFiles = new QAction(tr("Files"), this);
    QAction *pActionArchiveDir = new QAction(tr("Dir"), this);
    pMenuLibZipArchiveOpen->addAction(pActionArchiveFiles);
    pMenuLibZipArchiveOpen->addAction(pActionArchiveDir);
    pMenuLibZipArchiveOpen->setWindowFlags(pMenuLibZipArchiveOpen->windowFlags() | Qt::NoDropShadowWindowHint);
    this->connect(pActionArchiveFiles, SIGNAL(triggered(bool)), SLOT(slot_actionArchiveFiles_triggered(bool)));
    this->connect(pActionArchiveDir, SIGNAL(triggered(bool)), SLOT(slot_actionArchiveDir_triggered(bool)));
    ui.btnLibZipArchiveOpen->setMenu(pMenuLibZipArchiveOpen);
}

MiscellaneousZip::~MiscellaneousZip()
{
}

QString MiscellaneousZip::GetTitle()
{
    return QObject::tr("LibZip");
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

void MiscellaneousZip::ExtractFilesFromZip_SelfOpenFile(const QString sourceFile, const QString targetDir)
{
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

    zip_int64_t entryCount = zip_get_num_entries(pZipArchive, 0);
    if (entryCount < 0)
    {
        LogUtil::Error(CODE_LOCATION, "Can not get number of entries for %s. Error: %s", sourceFile.toUtf8().data(), zip_strerror(pZipArchive));
        zip_close(pZipArchive);
        return;
    }

    char readBuffer[8192] = { 0 };
    const char *pFilenameInZip = NULL;
    zip_int64_t readCount = 0, writeCount = 0;
    zip_error_t error_got;
    zip_error_t *pZipFileError = NULL;
    zip_file_t *pZipFileItem = NULL;
    zip_stat_t fileStatInZip;

    zip_stat_init(&fileStatInZip);
    zip_error_init(&error_got);

    for (int i = 0; i < entryCount; ++i)
    {
        zip_stat_index(pZipArchive, i, 0, &fileStatInZip);
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
                fwrite(readBuffer, sizeof(char), readCount, pExtractFile);
            }
            fclose(pExtractFile);

            //struct stat winFileStat;
            //stat(outFilename.toUtf8().data(), &winFileStat);

            LogUtil::Info(CODE_LOCATION, "%-20s size:%llu time:%lld CRC:%08X",
                          fileStatInZip.name, fileStatInZip.size, fileStatInZip.mtime, fileStatInZip.crc);

            struct utimbuf winTimeBuf;
            winTimeBuf.actime = fileStatInZip.mtime;
            winTimeBuf.modtime = fileStatInZip.mtime;
            utime(outFilename.toUtf8().data(), &winTimeBuf);

            if (readCount < 0)
            {
                pZipFileError = zip_file_get_error(pZipFileItem);
                zip_error_set(&error_got, zip_error_code_zip(pZipFileError), zip_error_code_system(pZipFileError));
            }
        }

        //zip_int64_t index;
        //index = zip_name_locate(pZipArchive, pFilenameInZip, ZIP_FL_NODIR);
        //if (index >= 0)
        //{
        //    zip_stat(pZipArchive, pFilenameInZip, 0, &fileStatInZip);
        //    LogUtil::Info(CODE_LOCATION, "%5d --> %s [%s] size:%8llu time:%lld CRC:%08X", index, pFilenameInZip,
        //                  fileStatInZip.name, fileStatInZip.size, fileStatInZip.mtime, fileStatInZip.crc);
        //}
        //else
        //{
        //    LogUtil::Warn(CODE_LOCATION, "%s does not exist in zip file.", pFilenameInZip);
        //}
    }

    zip_close(pZipArchive);
    zip_error_fini(&error_got);
}

void MiscellaneousZip::ExtractFilesFromZip_SelfOpenFileDir(const QString sourceFile, const QString targetDir)
{
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

    zip_int64_t entryCount = zip_get_num_entries(pZipArchive, 0);
    if (entryCount < 0)
    {
        LogUtil::Error(CODE_LOCATION, "Can not get number of entries for %s. Error: %s", sourceFile.toUtf8().data(), zip_strerror(pZipArchive));
        zip_close(pZipArchive);
        return;
    }

    char readBuffer[8192] = { 0 };
    const char *pFilenameInZip = NULL;
    zip_int64_t readCount = 0, writeCount = 0;
    zip_error_t error_got;
    zip_error_t *pZipFileError = NULL;
    zip_file_t *pZipFileItem = NULL;
    zip_stat_t fileStatInZip;

    zip_stat_init(&fileStatInZip);
    zip_error_init(&error_got);

    for (int i = 0; i < entryCount; ++i)
    {
        zip_stat_index(pZipArchive, i, 0, &fileStatInZip);
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
                fwrite(readBuffer, sizeof(char), readCount, pExtractFile);
            }
            fclose(pExtractFile);

            struct stat winFileStat;
            stat(outFilename.toUtf8().data(), &winFileStat);

            LogUtil::Info(CODE_LOCATION, "%-20s size:%llu time:%lld CRC:%08X",
                          fileStatInZip.name, fileStatInZip.size, fileStatInZip.mtime, fileStatInZip.crc);

            struct utimbuf winTimeBuf;
            winTimeBuf.actime = fileStatInZip.mtime;
            winTimeBuf.modtime = fileStatInZip.mtime;
            utime(outFilename.toUtf8().data(), &winTimeBuf);

            if (readCount < 0)
            {
                pZipFileError = zip_file_get_error(pZipFileItem);
                zip_error_set(&error_got, zip_error_code_zip(pZipFileError), zip_error_code_system(pZipFileError));
            }
        }
    }

    zip_close(pZipArchive);
    zip_error_fini(&error_got);
}

void MiscellaneousZip::ExtractFilesFromZip_ZipSourceFile(const QString sourceFile, const QString targetDir)
{
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
    zip_int64_t readCount = 0, writeCount = 0;
    zip_error_t error_got;
    zip_error_t *pZipFileError = NULL, *pSourceFileError = NULL;
    zip_file_t *pZipFileItem = NULL;
    zip_stat_t fileStatInZip;

    zip_stat_init(&fileStatInZip);
    zip_error_init(&error_got);

    for (int i = 0; i < entryCount; ++i)
    {
        zip_stat_index(pZipArchive, i, 0, &fileStatInZip);
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
            zip_source_t *pOutFile = zip_source_file_create(outFilename.toUtf8().data(), 0, 0, pSourceFileError);
            zip_source_begin_write(pOutFile);
            while ((readCount = zip_fread(pZipFileItem, readBuffer, sizeof(readBuffer))) > 0)
            {
                writeCount = zip_source_write(pOutFile, readBuffer, readCount);
                if (writeCount < 0)
                {
                    pSourceFileError = zip_source_error(pOutFile);
                    LogUtil::Debug(CODE_LOCATION, "write to source file failed: %s", zip_error_strerror(pSourceFileError));
                }
                else
                {
                    LogUtil::Debug(CODE_LOCATION, "readCount: %04d    writeCount: %04d", readCount, writeCount);
                }
            }
            zip_source_commit_write(pOutFile);
            zip_source_close(pOutFile);

            LogUtil::Info(CODE_LOCATION, "%-20s size:%llu time:%lld CRC:%08X",
                          fileStatInZip.name, fileStatInZip.size, fileStatInZip.mtime, fileStatInZip.crc);

            struct utimbuf winTimeBuf;
            winTimeBuf.actime = fileStatInZip.mtime;
            winTimeBuf.modtime = fileStatInZip.mtime;
            utime(outFilename.toUtf8().data(), &winTimeBuf);

            if (readCount < 0)
            {
                pZipFileError = zip_file_get_error(pZipFileItem);
                zip_error_set(&error_got, zip_error_code_zip(pZipFileError), zip_error_code_system(pZipFileError));
            }
        }
    }

    zip_close(pZipArchive);
    zip_error_fini(&error_got);
}

void MiscellaneousZip::ExtractFilesFromZip_ZipSourceFileDir(const QString sourceFile, const QString targetDir)
{
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
    zip_int64_t readCount = 0, writeCount = 0;
    zip_error_t error_got;
    zip_error_t *pZipFileError = NULL, *pSourceFileError = NULL;
    zip_file_t *pZipFileItem = NULL;
    zip_stat_t fileStatInZip;

    zip_stat_init(&fileStatInZip);
    zip_error_init(&error_got);

    for (int i = 0; i < entryCount; ++i)
    {
        zip_stat_index(pZipArchive, i, 0, &fileStatInZip);
        pFilenameInZip = zip_get_name(pZipArchive, i, ZIP_FL_ENC_GUESS);
        pZipFileItem = zip_fopen(pZipArchive, pFilenameInZip, ZIP_FL_ENC_GUESS);
        if (pZipFileItem == NULL)
        {
            pZipFileError = zip_get_error(pZipArchive);
            zip_error_set(&error_got, zip_error_code_zip(pZipFileError), zip_error_code_system(pZipFileError));
        }
        else
        {
            QString outFilename = targetDir + "/" + pFilenameInZip;
            int filenameLen = strlen(pFilenameInZip);
            if (pFilenameInZip[filenameLen-1] == '/')
            {
                mkdir(outFilename.toUtf8().data());
            }
            else
            {
                zip_source_t *pOutFile = zip_source_file_create(outFilename.toUtf8().data(), 0, 0, pSourceFileError);
                zip_source_begin_write(pOutFile);
                while ((readCount = zip_fread(pZipFileItem, readBuffer, sizeof(readBuffer))) > 0)
                {
                    writeCount = zip_source_write(pOutFile, readBuffer, readCount);
                    if (writeCount < 0)
                    {
                        pSourceFileError = zip_source_error(pOutFile);
                        LogUtil::Debug(CODE_LOCATION, "write to source file failed: %s", zip_error_strerror(pSourceFileError));
                    }
                    else
                    {
                        LogUtil::Debug(CODE_LOCATION, "readCount: %04d    writeCount: %04d", readCount, writeCount);
                    }
                }
                zip_source_commit_write(pOutFile);
                zip_source_close(pOutFile);

                // 保持文件原有时间戳
                struct utimbuf winTimeBuf;
                winTimeBuf.actime = fileStatInZip.mtime;
                winTimeBuf.modtime = fileStatInZip.mtime;
                utime(outFilename.toUtf8().data(), &winTimeBuf);
            }

            //LogUtil::Info(CODE_LOCATION, "%-20s size:%llu time:%lld CRC:%08X",
            //              fileStatInZip.name, fileStatInZip.size, fileStatInZip.mtime, fileStatInZip.crc);

            LogUtil::Info(CODE_LOCATION, "----------------------------------------\n%20s : %s\n%20s : %llu\n%20s : %llu\n%20s : %llu\n%20s : %lld\n%20s : %08X\n%20s : %d\n%20s : %d\n%20s : %lu\n----------------------------------------",
                          "name", fileStatInZip.valid & ZIP_STAT_NAME ? fileStatInZip.name : "NONE",
                          "index", fileStatInZip.valid & ZIP_STAT_INDEX ? fileStatInZip.index : 0,
                          "size", fileStatInZip.valid & ZIP_STAT_SIZE ? fileStatInZip.size : 0,
                          "comp_size", fileStatInZip.valid & ZIP_STAT_COMP_SIZE ? fileStatInZip.comp_size : 0,
                          "time", fileStatInZip.valid & ZIP_STAT_MTIME ? fileStatInZip.mtime : 0,
                          "CRC", fileStatInZip.valid & ZIP_STAT_CRC ? fileStatInZip.crc : 0,
                          "compression", fileStatInZip.valid & ZIP_STAT_COMP_METHOD ? fileStatInZip.comp_method : 0,
                          "enctyption", fileStatInZip.valid & ZIP_STAT_ENCRYPTION_METHOD ? fileStatInZip.encryption_method : 0,
                          "flags", fileStatInZip.valid & ZIP_STAT_FLAGS ? fileStatInZip.flags : 0);

            if (readCount < 0)
            {
                pZipFileError = zip_file_get_error(pZipFileItem);
                zip_error_set(&error_got, zip_error_code_zip(pZipFileError), zip_error_code_system(pZipFileError));
            }
        }
    }

    zip_close(pZipArchive);
    zip_error_fini(&error_got);
}

void MiscellaneousZip::ArchiveFilesToZip_SourceFiles(const QStringList sourceFiles, const QString targetFile)
{
    QFileInfo tempFileInfo;
    int errCode = ZIP_ER_OK;
    zip_t *pZipArchive = NULL;
    zip_source_t *pSourceFile = NULL;
    zip_error_t *pSourceFileError = NULL;
    zip_int64_t curIndex = 0;

    pZipArchive = zip_open(targetFile.toUtf8().data(), ZIP_CREATE, &errCode);
    if (pZipArchive == NULL)
    {
        zip_error_t error;
        zip_error_init_with_code(&error, errCode);
        LogUtil::Error(CODE_LOCATION, "Open file %s failed. Error[%d]: %s", targetFile.toUtf8().data(), errCode, zip_error_strerror(&error));
        zip_error_fini(&error);
        return;
    }

    for (QString sourceFile : sourceFiles)
    {
        tempFileInfo.setFile(sourceFile);
        pSourceFile = zip_source_file_create(sourceFile.toUtf8().data(), 0, 0, pSourceFileError);
        curIndex = zip_add(pZipArchive, tempFileInfo.fileName().toUtf8().data(), pSourceFile);
    }

    zip_close(pZipArchive);
}

void MiscellaneousZip::ArchiveFilesToZip_SourceDir(const QString sourceDir, const QString targetFile)
{
    int errCode = ZIP_ER_OK;
    zip_t *pZipArchive = NULL;
    zip_source_t *pSourceFile = NULL;
    zip_error_t *pSourceFileError = NULL;
    zip_int64_t curIndex = 0;

    pZipArchive = zip_open(targetFile.toUtf8().data(), ZIP_CREATE, &errCode);
    if (pZipArchive == NULL)
    {
        zip_error_t error;
        zip_error_init_with_code(&error, errCode);
        LogUtil::Error(CODE_LOCATION, "Open file %s failed. Error[%d]: %s", targetFile.toUtf8().data(), errCode, zip_error_strerror(&error));
        zip_error_fini(&error);
        return;
    }

    QDir sourceDirList;
    QFileInfo tempFileInfo;
    QFileInfoList sourceFileInfos;
    QStack<QFileInfo> sourceFileInfoStack;
    sourceFileInfoStack.push(QFileInfo(sourceDir));
    int rootDirLen = sourceDir.length();
    if (!(sourceDir.endsWith("/") || sourceDir.endsWith("\\")))
    {
        ++rootDirLen;
    }

    do
    {
        tempFileInfo = sourceFileInfoStack.pop();
        QString testFilename = tempFileInfo.fileName();
        QString testFilePath = tempFileInfo.filePath();
        QString testAbsolutePath = tempFileInfo.absolutePath();
        QString testAbsoluteFilePath = tempFileInfo.absoluteFilePath();

        if (tempFileInfo.isDir())
        {
            sourceDirList.setPath(tempFileInfo.filePath());
            sourceFileInfos = sourceDirList.entryInfoList(sourceDirList.filter() | QDir::NoDotAndDotDot);
            for (QFileInfo fileInfo : sourceFileInfos)
            {
                sourceFileInfoStack.push(fileInfo);
            }
            zip_dir_add(pZipArchive, tempFileInfo.filePath().mid(rootDirLen).toUtf8().data(), 0);
        }
        else
        {
            pSourceFile = zip_source_file_create(tempFileInfo.filePath().toUtf8().data(), 0, 0, pSourceFileError);
            curIndex = zip_add(pZipArchive, tempFileInfo.filePath().mid(rootDirLen).toUtf8().data(), pSourceFile);
        }
    }
    while (!sourceFileInfoStack.isEmpty());

    zip_close(pZipArchive);
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

    for (QString filename : sourceFiles)
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
    if (m_libZipArchiveType == ELZAOT_FILES)
    {
        QStringList sourceFiles = ui.leLibZipArchive->text().split(";");
        QString targetFile = ui.leLibZipSave->text();

        assert(sourceFiles.count() != 0);
        assert(!targetFile.isEmpty());
        if (sourceFiles.count() == 0 || targetFile.isEmpty()) return;
        this->ArchiveFilesToZip_SourceFiles(sourceFiles, targetFile);
    } 
    else if (m_libZipArchiveType == ELZAOT_DIR)
    {
        QString sourceDir = ui.leLibZipArchive->text();
        QString targetFile = ui.leLibZipSave->text();

        assert(!sourceDir.isEmpty());
        assert(!targetFile.isEmpty());
        if (sourceDir.isEmpty() || targetFile.isEmpty()) return;
        this->ArchiveFilesToZip_SourceDir(sourceDir, targetFile);
    }
}

void MiscellaneousZip::slot_actionArchiveFiles_triggered(bool checked)
{
    m_libZipArchiveType = ELZAOT_FILES;
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open files"), qApp->applicationDirPath(), "All (*);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)");
    ui.leLibZipArchive->setText(files.join(";"));;
}

void MiscellaneousZip::slot_actionArchiveDir_triggered(bool checked)
{
    m_libZipArchiveType = ELZAOT_DIR;
    QString sourceDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leLibZipArchive->setText(sourceDir);
}

void MiscellaneousZip::on_btnLibZipArchiveBrowse_clicked()
{
    QString zipFileSave = QFileDialog::getSaveFileName(this, tr("Save Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leLibZipSave->setText(zipFileSave);
}

void MiscellaneousZip::on_btnLibZipExtract_clicked()
{
    QString sourceFile = ui.leLibZipOpen->text();
    QString targetDir = ui.leLibZipExtract->text();

    assert(!sourceFile.isEmpty());
    assert(!targetDir.isEmpty());
    if (sourceFile.isEmpty() || targetDir.isEmpty()) return;

    //this->ExtractFilesFromZip_SelfOpenFile(sourceFile, targetDir);
    //this->ExtractFilesFromZip_SelfOpenFileDir(sourceFile, targetDir);
    //this->ExtractFilesFromZip_ZipSourceFile(sourceFile, targetDir);
    this->ExtractFilesFromZip_ZipSourceFileDir(sourceFile, targetDir);
}

void MiscellaneousZip::on_btnLibZipExtractOpen_clicked()
{
    QString zipFileOpen = QFileDialog::getOpenFileName(this, tr("Open Zip file"), qApp->applicationDirPath(), "Zip (*.zip);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All (*)");
    ui.leLibZipOpen->setText(zipFileOpen);
}

void MiscellaneousZip::on_btnLibZipExtractBrowse_clicked()
{
    QString extractDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leLibZipExtract->setText(extractDir);
}
