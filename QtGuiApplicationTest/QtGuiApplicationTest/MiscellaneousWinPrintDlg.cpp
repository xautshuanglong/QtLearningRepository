#include "MiscellaneousWinPrintDlg.h"

#include <QWindow>

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
    return MiscellaneousTestItem::WinAPI_Thread_WinPrintDlg;
}

void MiscellaneousWinPrintDlg::on_btnPrintDlg_clicked()
{
    PRINTDLG pd;
    // Initialize PRINTDLG
    ZeroMemory(&pd, sizeof(pd));
    pd.lStructSize = sizeof(pd);
    pd.hwndOwner = (HWND)this->nativeParentWidget()->windowHandle()->winId();
    pd.hDevMode = NULL;      // Don't forget to free or store hDevMode.
    pd.hDevNames = NULL;     // Don't forget to free or store hDevNames.
    pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
    pd.nCopies = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage = 0xFFFF;
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;

    if (PrintDlg(&pd) == TRUE)
    {
        // GDI calls to render output. 

        // Delete DC when done.
        DeleteDC(pd.hDC);
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
    pdx.Flags = PD_RETURNDC | PD_COLLATE;
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
    pdx.lpCallback = NULL;
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
        GlobalFree(pdx.hDevMode);
    if (pdx.hDevNames != NULL)
        GlobalFree(pdx.hDevNames);
    if (pdx.lpPageRanges != NULL)
        GlobalFree(pPageRanges);

    if (pdx.hDC != NULL)
        DeleteDC(pdx.hDC);
}

void MiscellaneousWinPrintDlg::on_btnPageSetup_clicked()
{
    PAGESETUPDLG psd;      // common dialog box structure
    //HWND hwnd;           // owner window

    // Initialize PAGESETUPDLG
    ZeroMemory(&psd, sizeof(psd));
    psd.lStructSize = sizeof(psd);
    // QT 中的实现
    //psd.hwndOwner = parentWindow ? (HWND)QGuiApplication::platformNativeInterface()->nativeResourceForWindow("handle", parentWindow)
    psd.hwndOwner = (HWND)this->nativeParentWidget()->windowHandle()->winId();
    psd.hDevMode = NULL;  // Don't forget to free or store hDevMode.
    psd.hDevNames = NULL; // Don't forget to free or store hDevNames.
    psd.Flags = PSD_INHUNDREDTHSOFMILLIMETERS | PSD_MARGINS | PSD_ENABLEPAGEPAINTHOOK; // PSD_INTHOUSANDTHSOFINCHES
    psd.rtMargin.top = 1000;
    psd.rtMargin.left = 1250;
    psd.rtMargin.right = 1250;
    psd.rtMargin.bottom = 1000;
    psd.lpfnPagePaintHook = MiscellaneousWinPrintDlg::PaintHook;

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

UINT_PTR CALLBACK MiscellaneousWinPrintDlg::PaintHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
