#ifndef MISCELLANEOUS_PRINTER_WIDGET_H
#define MISCELLANEOUS_PRINTER_WIDGET_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousPrinterWidget.h"

class MiscellaneousPrinterWidget : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousPrinterWidget(QWidget *parent = 0);
    ~MiscellaneousPrinterWidget();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    Ui::MiscellaneousPrinterWidget ui;
};

#endif // MISCELLANEOUS_PRINTER_WIDGET_H
