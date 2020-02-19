#ifndef MISCELLANEOUS_ZLIB_H
#define MISCELLANEOUS_ZLIB_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousZLib.h"

class MiscellaneousZLib : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousZLib(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousZLib();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    int def(FILE *source, FILE *dest, int level);
    int inf(FILE *source, FILE *dest);
    void zerr(int ret);

private slots:
    void on_btnBrowseFiles_clicked();
    void on_btnBrowseDirectory_clicked();
    void on_btnFilesDeflate_clicked();
    void on_btnFilesInflate_clicked();
    void on_btnDirectoryDeflate_clicked();
    void on_btnDirectoryInflate_clicked();

private:
    Ui::MiscellaneousZLib ui;
};

#endif
