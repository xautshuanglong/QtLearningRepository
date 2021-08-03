#ifndef MISCELLANEOUS_ZIP_H
#define MISCELLANEOUS_ZIP_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousZip.h"

class MiscellaneousZip : public MiscellaneousBase
{
    Q_OBJECT

    enum EnumLibZipArchiveOpenType
    {
        ELZAOT_UNKNOWN,
        ELZAOT_FILES,
        ELZAOT_DIR
    };

public:
    MiscellaneousZip(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousZip();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void ExtractFilesFromZip_SelfOpenFile(const QString sourceFile, const QString targetDir);
    void ExtractFilesFromZip_SelfOpenFileDir(const QString sourceFile, const QString targetDir);
    void ExtractFilesFromZip_ZipSourceFile(const QString sourceFile, const QString targetDir);
    void ExtractFilesFromZip_ZipSourceFileDir(const QString sourceFile, const QString targetDir);
    void ArchiveFilesToZip_SourceFiles(const QStringList sourceFiles, const QString targetFile);
    void ArchiveFilesToZip_SourceDir(const QString sourceDir, const QString targetFile);

private slots:
    void on_btnFilesArchive_clicked();
    void on_btnFilesArchiveOpen_clicked();
    void on_btnFilesArchiveBrowse_clicked();
    void on_btnFilesExtract_clicked();
    void on_btnFilesExtractOpen_clicked();
    void on_btnFilesExtractBrowse_clicked();

    void on_btnDirectoryArchive_clicked();
    void on_btnDirectoryArchiveOpen_clicked();
    void on_btnDirectoryArchiveBrowse_clicked();
    void on_btnDirectoryExtract_clicked();
    void on_btnDirectoryExtractOpen_clicked();
    void on_btnDirectoryExtractBrowse_clicked();

    void slot_actionArchiveFiles_triggered(bool checked);
    void slot_actionArchiveDir_triggered(bool checked);
    void on_btnLibZipArchive_clicked();
    void on_btnLibZipArchiveBrowse_clicked();
    void on_btnLibZipExtract_clicked();
    void on_btnLibZipExtractOpen_clicked();
    void on_btnLibZipExtractBrowse_clicked();

private:
    Ui::MiscellaneousZip ui;
    EnumLibZipArchiveOpenType m_libZipArchiveType;
};

#endif // MISCELLANEOUS_ZIP_H
