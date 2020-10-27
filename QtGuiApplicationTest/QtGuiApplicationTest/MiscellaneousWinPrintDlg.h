#ifndef MISCELLANEOUS_WIN_PRINT_DLG_H
#define MISCELLANEOUS_WIN_PRINT_DLG_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousWinPrintDlg.h"

#include "windows.h"

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

class WinPrintDialogExCallback : public IPrintDialogCallback
{
public:
    explicit WinPrintDialogExCallback();
    ~WinPrintDialogExCallback();

    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ _In_ REFIID riid, _Outptr_ void** ppvObj) override;
    STDMETHOD_(ULONG, AddRef) (THIS) override;
    STDMETHOD_(ULONG, Release)(THIS) override;
    // *** IPrintDialogCallback methods ***
    STDMETHOD(InitDone) (THIS) override;
    STDMETHOD(SelectionChange) (THIS) override;
    STDMETHOD(HandleMessage) (THIS_ HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;

private:
    ULONG     mRefCount;
};

class WinPrintDialogExService : public IPrintDialogServices
{
public:
    explicit WinPrintDialogExService();
    ~WinPrintDialogExService();

    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ _In_ REFIID riid, _Outptr_ void** ppvObj) override;
    STDMETHOD_(ULONG, AddRef) (THIS) override;
    STDMETHOD_(ULONG, Release)(THIS) override;
    // *** IPrintDialogServices methods ***
    STDMETHOD(GetCurrentDevMode) (THIS_ _Inout_ LPDEVMODE pDevMode, _Inout_ UINT* pcbSize) override;
    STDMETHOD(GetCurrentPrinterName) (THIS_ _Out_writes_opt_(*pcchSize) LPWSTR pPrinterName, _Inout_ UINT* pcchSize) override;
    STDMETHOD(GetCurrentPortName) (THIS_ _Out_writes_opt_(*pcchSize) LPWSTR pPortName, _Inout_ UINT* pcchSize) override;

private:
    ULONG       mRefCount;
};

#endif // MISCELLANEOUS_WIN_PRINT_DLG_H
