﻿#include "MiscellaneousWinPrintDlg.h"

#include <QWindow>

#pragma warning (disable: 6387)

MiscellaneousWinPrintDlg::MiscellaneousWinPrintDlg(QWidget *parent /* = Q_NULLPTR */)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousWinPrintDlg::~MiscellaneousWinPrintDlg()
{
}

QString MiscellaneousWinPrintDlg::GetTitle()
{
    return QObject::tr("Win Print Dialog");
}

QString MiscellaneousWinPrintDlg::GetTitleTooltip()
{
    return QObject::tr("How to use windows API setup printer properties.");
}

MiscellaneousTestGroup MiscellaneousWinPrintDlg::GetGroupID()
{
    return MiscellaneousTestGroup::WinAPI_Test;
}

MiscellaneousTestItem MiscellaneousWinPrintDlg::GetItemID()
{
    return MiscellaneousTestItem::WinAPI_Print_Dialog;
}

void MiscellaneousWinPrintDlg::on_btnPrintDlg_clicked()
{
    PRINTDLG pd;
    // Initialize PRINTDLG
    ZeroMemory(&pd, sizeof(pd)); // 下列置空参数，用于标记改结构体有哪些可用参数。
    pd.lStructSize = sizeof(pd);
    pd.hwndOwner = (HWND)this->nativeParentWidget()->windowHandle()->winId();
    pd.hDevMode = NULL;      // Don't forget to free or store hDevMode.
    pd.hDevNames = NULL;     // Don't forget to free or store hDevNames.
    pd.hDC = NULL;
    //pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_ENABLEPRINTHOOK | PD_ENABLESETUPHOOK | PD_RETURNDC;
    pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
    pd.nFromPage = 0xFFFF;
    pd.nToPage = 0xFFFF;
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;
    pd.nCopies = 1;
    pd.hInstance = NULL;
    pd.lCustData = 0;
    pd.lpfnPrintHook = MiscellaneousWinPrintDlg::PrintDlg_PrintHook;
    pd.lpfnSetupHook = MiscellaneousWinPrintDlg::PrintDlg_SetupHook;
    pd.lpPrintTemplateName = TEXT("");
    pd.lpSetupTemplateName = TEXT("");
    pd.hPrintTemplate = NULL;
    pd.hSetupTemplate = NULL;

    if (PrintDlg(&pd) == TRUE)
    {
        // GDI calls to render output. 

        // Delete DC when done.
        if (pd.hDC != NULL)
        {
            DeleteDC(pd.hDC);
        }
    }
    else
    {
        DWORD errCode = CommDlgExtendedError();
        if (errCode != 0)
        {
            int i = 0;
        }
    }
}

void MiscellaneousWinPrintDlg::on_btnPrintDlgEx_clicked()
{
    HRESULT hResult;
    PRINTDLGEX pdx = { 0 };
    LPPRINTPAGERANGE pPageRanges = NULL;

    // Allocate an array of PRINTPAGERANGE structures.
    pPageRanges = (LPPRINTPAGERANGE)GlobalAlloc(GPTR, 10 * sizeof(PRINTPAGERANGE));
    if (!pPageRanges)
        return;

    //  Initialize the PRINTDLGEX structure.
    pdx.lStructSize = sizeof(PRINTDLGEX);
    pdx.hwndOwner = (HWND)this->nativeParentWidget()->windowHandle()->winId();
    pdx.hDevMode = NULL;
    pdx.hDevNames = NULL;
    pdx.hDC = NULL;
    pdx.Flags = PD_RETURNDC | PD_USEDEVMODECOPIESANDCOLLATE;
    pdx.Flags2 = 0;
    pdx.ExclusionFlags = 0;
    pdx.nPageRanges = 0;
    pdx.nMaxPageRanges = 10;
    pdx.lpPageRanges = pPageRanges;
    pdx.nMinPage = 1;
    pdx.nMaxPage = 1000;
    pdx.nCopies = 1;
    pdx.hInstance = 0;
    pdx.lpPrintTemplateName = NULL;
    pdx.lpCallback = (IPrintDialogCallback*) new WinPrintDialogExCallback();
    pdx.nPropertyPages = 0;
    pdx.lphPropertyPages = NULL;
    pdx.nStartPage = START_PAGE_GENERAL;
    pdx.dwResultAction = 0;

    //  Invoke the Print property sheet.
    hResult = PrintDlgEx(&pdx);
    if ((hResult == S_OK) && pdx.dwResultAction == PD_RESULT_PRINT)
    {
        // User clicked the Print button, so use the DC and other information returned in the 
        // PRINTDLGEX structure to print the document.
    }
    else if (hResult != S_OK)
    {
        DWORD errCode = CommDlgExtendedError();
        if (errCode != 0)
        {
            int i = 0;
        }
    }

    if (pdx.hDevMode != NULL)
    {
        DEVMODE* pDevMode = nullptr;
        pDevMode = reinterpret_cast<DEVMODE*>(GlobalLock(pdx.hDevMode));
        GlobalFree(pdx.hDevMode);
    }
    if (pdx.hDevNames != NULL)
    {
        DEVNAMES* pDevNames = nullptr;
        pDevNames = reinterpret_cast<DEVNAMES*>(GlobalLock(pdx.hDevNames));
        PCSTR printerName = reinterpret_cast<PCSTR>(pDevNames) + pDevNames->wDeviceOffset;
        GlobalFree(pdx.hDevNames);
    }
    if (pdx.lpPageRanges != NULL)
    {
        GlobalFree(pPageRanges);
    }
    if (pdx.hDC != NULL)
    {
        DeleteDC(pdx.hDC);
    }
}

void MiscellaneousWinPrintDlg::on_btnPageSetup_clicked()
{
    PAGESETUPDLG psd;      // common dialog box structure
    //HWND hwnd;           // owner window

    // Initialize PAGESETUPDLG
    ZeroMemory(&psd, sizeof(psd));
    psd.lStructSize = sizeof(psd);
    psd.hwndOwner = (HWND)this->nativeParentWidget()->windowHandle()->winId(); //psd.hwndOwner = parentWindow ? (HWND)QGuiApplication::platformNativeInterface()->nativeResourceForWindow("handle", parentWindow)
    psd.hDevMode = NULL;  // Don't forget to free or store hDevMode.
    psd.hDevNames = NULL; // Don't forget to free or store hDevNames.
    psd.Flags = PSD_INHUNDREDTHSOFMILLIMETERS | PSD_MARGINS | PSD_ENABLEPAGESETUPHOOK | PSD_ENABLEPAGEPAINTHOOK; // PSD_INTHOUSANDTHSOFINCHES
    psd.ptPaperSize = { 0, 0 };
    psd.rtMinMargin = { 0, 0, 0, 0 };          // left  top  right  bottom;
    psd.rtMargin = { 1250, 1000, 1250, 1000 }; // left  top  right  bottom;
    psd.hInstance = NULL;
    psd.lCustData = 0;
    psd.lpfnPageSetupHook = MiscellaneousWinPrintDlg::PageSetupDlg_SetupHook;
    psd.lpfnPagePaintHook = MiscellaneousWinPrintDlg::PageSetupDlg_PaintHook;
    psd.lpPageSetupTemplateName = TEXT("");
    psd.hPageSetupTemplate = NULL;

    if (PageSetupDlg(&psd) == TRUE)
    {
        // check paper size and margin values here.
        int i = 0;
    }
    else
    {
        DWORD errCode = CommDlgExtendedError();
        if (errCode != 0)
        {
            int i = 0;
        }
    }
}

void MiscellaneousWinPrintDlg::on_btnEmpty1_clicked()
{
    int i = 0;
}

UINT_PTR CALLBACK MiscellaneousWinPrintDlg::PrintDlg_SetupHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return FALSE;
}
UINT_PTR CALLBACK MiscellaneousWinPrintDlg::PrintDlg_PrintHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return FALSE;
}

UINT_PTR CALLBACK MiscellaneousWinPrintDlg::PageSetupDlg_SetupHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPRECT lprc;
    COLORREF penColor;
    HDC hdc;
    HFONT hFont;
    HGDIOBJ hdcOld;

    // 通用对话框消息，一般无需特殊处理。
    switch (uMsg)
    {
    default:
        return FALSE;
    }
    return FALSE;
}

// 绘制打印室实例（页面设置对话框内）
UINT_PTR CALLBACK MiscellaneousWinPrintDlg::PageSetupDlg_PaintHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPRECT lprc;
    COLORREF penColor;
    HDC hdc;
    HFONT hFont;
    HGDIOBJ hdcOld;

    switch (uMsg)
    {
        // Draw the margin rectangle. 
    case WM_PSD_MARGINRECT:
        hdc = (HDC)wParam;
        lprc = (LPRECT)lParam;
        // Get the system highlight color. 
        penColor = GetSysColor(COLOR_HIGHLIGHT);
        // Create a dash-dot pen of the system highlight color and 
        // select it into the DC of the sample page. 
        hdcOld = SelectObject(hdc, CreatePen(PS_DASHDOT, 1, penColor));
        // Draw the margin rectangle. 
        Rectangle(hdc, lprc->left, lprc->top, lprc->right, lprc->bottom);
        // Restore the previous pen to the DC. 
        SelectObject(hdc, hdcOld);
        return TRUE;

        // Draw greek text rectangle
    case WM_PSD_GREEKTEXTRECT:
        hdc = (HDC)wParam;
        lprc = (LPRECT)lParam;
        hFont = CreateFont(10, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                           CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("楷体"));
        hdcOld = SelectObject(hdc, hFont);
        DrawText(hdc, TEXT("Hello world!\n你好世界！"), -1, lprc, DT_CENTER | DT_VCENTER);
        DeleteObject(hFont);
        SelectObject(hdc, hdcOld);
        return TRUE;

    default:
        return FALSE;
    }
    return TRUE;
}

WinPrintDialogExCallback::WinPrintDialogExCallback()
    : mnRefCount(0)
    , mpSelfPointer(this)
{
    int i = 0;
}

WinPrintDialogExCallback::~WinPrintDialogExCallback()
{
    int i = 0;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE WinPrintDialogExCallback::QueryInterface(THIS_ _In_ REFIID riid, _Outptr_ void** ppvObj)
{
    if (ppvObj == nullptr) return E_POINTER;

    HRESULT retValue = E_NOINTERFACE;
    if (riid == IID_IUnknown)
    {
        *ppvObj = static_cast<IUnknown*>(static_cast<IPrintDialogCallback*>(this));
        this->AddRef();
        retValue = S_OK;
    }
    else if (riid == IID_IPrintDialogCallback)
    {
        *ppvObj = static_cast<IPrintDialogCallback*>(this);
        this->AddRef();
        retValue = S_OK;
    }
    else if (riid == IID_IObjectWithSite)
    {
        *ppvObj = static_cast<IObjectWithSite*>(this);
        this->AddRef();
        retValue = S_OK;
    }
    return retValue;
}

COM_DECLSPEC_NOTHROW ULONG STDMETHODCALLTYPE WinPrintDialogExCallback::AddRef(THIS)
{
    if (mnRefCount < ULONG_MAX)
    {
        ++mnRefCount;
    }
    return mnRefCount;
}

COM_DECLSPEC_NOTHROW ULONG STDMETHODCALLTYPE WinPrintDialogExCallback::Release(THIS)
{
    if (mnRefCount > 0)
    {
        --mnRefCount;
    }
    if (mnRefCount == 0 && mpSelfPointer != nullptr)
    {
        delete mpSelfPointer;
        mpSelfPointer = nullptr;
    }
    return mnRefCount;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE WinPrintDialogExCallback::InitDone(THIS)
{
    return TRUE;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE WinPrintDialogExCallback::SelectionChange(THIS)
{
    return TRUE;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE WinPrintDialogExCallback::HandleMessage(THIS_ HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    return TRUE;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE WinPrintDialogExCallback::SetSite(__RPC__in_opt IUnknown* pUnkSite)
{
    if (pUnkSite == NULL) return E_POINTER;

    IPrintDialogServices* pPrintServices = NULL;
    HRESULT res = pUnkSite->QueryInterface(IID_IPrintDialogServices, (void**)&pPrintServices);
    if (res == S_OK)
    {
        WCHAR printerNameArr[128] = { 0 };
        LPCSTR printerName = (LPCSTR)printerNameArr;
        UINT printerNameLen = 128;
        HRESULT printerRes = pPrintServices->GetCurrentPrinterName(printerNameArr, &printerNameLen);
        
        WCHAR portNameArr[128] = { 0 };
        LPCSTR portName = (LPCSTR)portNameArr;
        UINT portNameLen = 128;
        printerRes = pPrintServices->GetCurrentPortName(portNameArr, &portNameLen);

        UINT devModeSize = 0;
        printerRes = pPrintServices->GetCurrentDevMode(NULL, &devModeSize);
        if (devModeSize > 0)
        {
            DEVMODE* pDevMode = (DEVMODE*)GlobalAlloc(GPTR, devModeSize * sizeof(char));
            if (pDevMode != NULL)
            {
                printerRes = pPrintServices->GetCurrentDevMode(pDevMode, &devModeSize);
                GlobalFree(pDevMode);
            }
        }

        pPrintServices->Release();
    }

    return S_OK;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE WinPrintDialogExCallback::GetSite(REFIID riid, void** ppvSite)
{
    if (ppvSite == NULL) return E_POINTER;

    HRESULT retValue = E_NOINTERFACE;
    if (ppvSite != NULL)
    {
        if (riid == IID_IUnknown)
        {
            *ppvSite = nullptr;
            retValue = S_OK;
        }
        else if (riid == IID_IPrintDialogCallback)
        {
            *ppvSite = nullptr;
            retValue = S_OK;
        }
        else if (riid == IID_IPrintDialogServices)
        {
            *ppvSite = nullptr;
            retValue = S_OK;
        }
    }
    return retValue;
}
