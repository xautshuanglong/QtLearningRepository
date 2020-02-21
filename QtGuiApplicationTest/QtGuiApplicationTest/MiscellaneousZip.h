#ifndef MISCELLANEOUS_ZIP_H
#define MISCELLANEOUS_ZIP_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousZip.h"

class MiscellaneousZip : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousZip(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousZip();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnBrowseFiles_clicked();
    void on_btnBrowseDirectory_clicked();
    void on_btnFilesArchive_clicked();
    void on_btnFilesExtract_clicked();
    void on_btnDirectoryArchive_clicked();
    void on_btnDirectoryExtract_clicked();

private:
    Ui::MiscellaneousZip ui;
};

#endif // MISCELLANEOUS_ZIP_H
