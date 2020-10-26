#include "MiscellaneousWinPrintDlg.h"

#include <windows.h>
#include <process.h>

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

void MiscellaneousWinPrintDlg::on_btnPrinterOptions_clicked()
{
    int i = 0;
}

void MiscellaneousWinPrintDlg::on_btnPageSetup_clicked()
{
    int i = 0;
}

void MiscellaneousWinPrintDlg::on_btnEmpty1_clicked()
{
    int i = 0;
}

void MiscellaneousWinPrintDlg::on_btnEmpty2_clicked()
{
    int i = 0;
}
