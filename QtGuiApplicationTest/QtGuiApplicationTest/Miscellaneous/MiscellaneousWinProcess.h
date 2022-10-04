#ifndef MISCELLANEOUS_WIN_PROCESS_H
#define MISCELLANEOUS_WIN_PROCESS_H

#include "MiscellaneousBase.h"

#include <string>
#include <thread>

#include <Windows.h>

namespace Ui { class MiscellaneousWinProcess; };

class MiscellaneousWinProcess : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinProcess(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinProcess();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnEnumProcess_clicked();
    void on_btnEnumModule_clicked();

private:
    void PSAPI_EnumProcess();
    void PSAPI_PrintProcessName(DWORD processID);
    void PSAPI_PrintProcessModule(DWORD processID);

private:
    Ui::MiscellaneousWinProcess *ui;
};

#endif // MISCELLANEOUS_WIN_PROCESS_H
