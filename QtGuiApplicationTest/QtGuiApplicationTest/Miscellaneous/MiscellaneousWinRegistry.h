#ifndef MISCELLANEOUS_WIN_REGISTRY_H
#define MISCELLANEOUS_WIN_REGISTRY_H

#include "MiscellaneousBase.h"

#include <string>
#include <thread>

namespace Ui { class MiscellaneousWinRegistry; };

class MiscellaneousWinRegistry : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinRegistry(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinRegistry();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnMonitorRegStart_clicked();
    void on_btnMonitorRegStop_clicked();
    void on_btnMonitorRegSingle_clicked();
    void on_btnEventWrite_clicked();
    void on_btnEmptyTest_clicked();

private:
    void MonitorRegChange_RegNotifyChangeValue(const std::string& mainKey, const std::string& subKey);
    void MonitorRegChange_RegNotifyChangeValueLoop(const std::string& mainKey, const std::string& subKey);
    void MonitorRegChange_RegNotifyChangeValueMultiple();

private:
    Ui::MiscellaneousWinRegistry *ui;
    bool mIsMonitorRegRunning;
    std::thread* mpRegMonitorThread;
};

#endif // MISCELLANEOUS_WIN_REGISTRY_H
