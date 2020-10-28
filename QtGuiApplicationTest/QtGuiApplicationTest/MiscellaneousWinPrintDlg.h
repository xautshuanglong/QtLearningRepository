#ifndef MISCELLANEOUS_WIN_PRINT_DLG_H
#define MISCELLANEOUS_WIN_PRINT_DLG_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousWinPrintDlg.h"

#include <windows.h>
#include <OCIdl.h>

class MiscellaneousWinPrintDlg : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinPrintDlg(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinPrintDlg();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    static UINT_PTR CALLBACK PrintDlg_SetupHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static UINT_PTR CALLBACK PrintDlg_PrintHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static UINT_PTR CALLBACK PageSetupDlg_SetupHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static UINT_PTR CALLBACK PageSetupDlg_PaintHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private slots:
    void on_btnPrintDlg_clicked();
    void on_btnPrintDlgEx_clicked();
    void on_btnPageSetup_clicked();
    void on_btnEmpty1_clicked();

private:
    Ui::MiscellaneousWinPrintDlg   ui;
};

class WinPrintDialogExCallback : public IPrintDialogCallback, public IObjectWithSite
{
public:
    explicit WinPrintDialogExCallback();
    virtual ~WinPrintDialogExCallback();

    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ _In_ REFIID riid, _Outptr_ void** ppvObj) override;
    STDMETHOD_(ULONG, AddRef) (THIS) override;
    STDMETHOD_(ULONG, Release)(THIS) override;
    // *** IPrintDialogCallback methods ***
    STDMETHOD(InitDone) (THIS) override;
    STDMETHOD(SelectionChange) (THIS) override;
    STDMETHOD(HandleMessage) (THIS_ HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;
    // *** IObjectWithSite methods ***
    STDMETHOD(SetSite) (__RPC__in_opt IUnknown* pUnkSite) override;
    STDMETHOD(GetSite) (__RPC__in REFIID riid, __RPC__deref_out_opt void** ppvSite) override;

private:
    ULONG                     mnRefCount;
    WinPrintDialogExCallback *mpSelfPointer;
};

#endif // MISCELLANEOUS_WIN_PRINT_DLG_H
