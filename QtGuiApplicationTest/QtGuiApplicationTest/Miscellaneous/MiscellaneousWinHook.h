#ifndef MISCELLANEOUS_WIN_HOOK_H
#define MISCELLANEOUS_WIN_HOOK_H

#include "MiscellaneousBase.h"

#include <string>
#include <thread>

namespace Ui { class MiscellaneousWinHook; };

class MiscellaneousWinHook : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinHook(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinHook();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnWindowsHookStart_clicked();
    void on_btnWindowsHookStop_clicked();

private:
    void MonitorRegChange_RegNotifyChangeValue(const std::string& mainKey, const std::string& subKey);
    void MonitorRegChange_RegNotifyChangeValueLoop(const std::string& mainKey, const std::string& subKey);
    void MonitorRegChange_RegNotifyChangeValueMultiple();

private:
    Ui::MiscellaneousWinHook *ui;
    bool mIsMonitorRegRunning;
    std::thread* mpRegMonitorThread;
};

#endif // MISCELLANEOUS_WIN_HOOK_H
