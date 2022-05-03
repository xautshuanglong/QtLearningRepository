#ifndef MISCELLANEOUS_WIN_SYSTEM_INFO_H
#define MISCELLANEOUS_WIN_SYSTEM_INFO_H

#include <mutex>
#include <shared_mutex>

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousWinSystemInfo.h"

class MiscellaneousWinSystemInfo : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinSystemInfo(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinSystemInfo();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:

private slots:
    void on_btnSystemInfo_clicked();
    void on_btnTotalMemory_clicked();
    void on_btnEmptyTest1_clicked();
    void on_btnEmptyTest2_clicked();

private:
    Ui::MiscellaneousWinSystemInfo ui;
};

#endif // MISCELLANEOUS_WIN_SYSTEM_INFO_H
