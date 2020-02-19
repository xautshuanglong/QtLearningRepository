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

private slots:
    void on_btnBrowseFiles_clicked();
    void on_btnBrowseDirectory_clicked();
    void on_btnArchiveFiles_clicked();
    void on_btnArchiveDirectory_clicked();

private:
    Ui::MiscellaneousZLib ui;
};

#endif
