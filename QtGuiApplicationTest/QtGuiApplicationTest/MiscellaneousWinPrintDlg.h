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
    static UINT_PTR CALLBACK PaintHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private slots:
    void on_btnPrintDlg_clicked();
    void on_btnPrintDlgEx_clicked();
    void on_btnPageSetup_clicked();
    void on_btnEmpty1_clicked();

private:
    Ui::MiscellaneousWinPrintDlg   ui;
};

#endif // MISCELLANEOUS_WIN_PRINT_DLG_H
