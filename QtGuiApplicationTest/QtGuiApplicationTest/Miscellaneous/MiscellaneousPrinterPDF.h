#ifndef MISCELLANEOUS_QT_PRINTER_H
#define MISCELLANEOUS_QT_PRINTER_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousPrinterPDF.h"

class QPrinter;
class QPrintPreviewWidget;

class MiscellaneousPrinterPDF : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousPrinterPDF(QWidget *parent = 0);
    ~MiscellaneousPrinterPDF();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void SlotPreviewPaintRequested(QPrinter* printer);
    void on_btnPrinterInfo_clicked();
    void on_btnPageSetupDlg_clicked();
    void on_btnPrintPreviewDlg_clicked();
    void on_btnPrintPreviewWgt_clicked();
    void on_btnPrintDlg_clicked();
    void on_btnQPrinterTest_clicked();
    void on_btnQPrinterCustom_clicked();

private:
    Ui::MiscellaneousPrinterPDF ui;
    QPrintPreviewWidget        *mpPrintPreviewWgt;
};

#endif // MISCELLANEOUS_QT_PRINTER_H
