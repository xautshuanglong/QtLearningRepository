#ifndef MISCELLANEOUS_QT_PRINTER_H
#define MISCELLANEOUS_QT_PRINTER_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousPrinterPDF.h"

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

private:
    Ui::MiscellaneousPrinterPDF ui;

private slots:
    void on_btnQPrinterTest_clicked();
};

#endif // MISCELLANEOUS_QT_PRINTER_H
