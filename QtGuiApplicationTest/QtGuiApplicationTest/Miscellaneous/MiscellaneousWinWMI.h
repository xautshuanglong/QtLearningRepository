#ifndef MISCELLANEOUS_WIN_WMI_H
#define MISCELLANEOUS_WIN_WMI_H

#include "MiscellaneousBase.h"

#include <string>
#include <thread>

namespace Ui { class MiscellaneousWinWMI; };

class MiscellaneousWinWMI : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinWMI(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinWMI();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnMonitorRegStart_clicked();
    void on_btnMonitorRegStop_clicked();
    void on_btnEventWrite_clicked();
    void on_btnEmptyTest_clicked();

private:

private:
    Ui::MiscellaneousWinWMI *ui;
    bool mIsMonitorRegRunning;
    std::thread* mpRegMonitorThread;
};

#endif // MISCELLANEOUS_WIN_WMI_H
