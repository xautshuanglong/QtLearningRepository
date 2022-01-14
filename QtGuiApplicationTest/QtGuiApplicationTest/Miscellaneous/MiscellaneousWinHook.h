#ifndef MISCELLANEOUS_WIN_HOOK_H
#define MISCELLANEOUS_WIN_HOOK_H

#include "MiscellaneousBase.h"

#include <string>
#include <thread>

#include <Windows.h>

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

public:
    static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK DebugProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK MessageProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    Ui::MiscellaneousWinHook *ui;
};

#endif // MISCELLANEOUS_WIN_HOOK_H
