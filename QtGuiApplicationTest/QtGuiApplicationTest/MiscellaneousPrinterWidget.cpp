#include "MiscellaneousPrinterWidget.h"

MiscellaneousPrinterWidget::MiscellaneousPrinterWidget(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousPrinterWidget::~MiscellaneousPrinterWidget()
{
}

QString MiscellaneousPrinterWidget::GetTitle()
{
    return QObject::tr("Widget Printer");
}

QString MiscellaneousPrinterWidget::GetTitleTooltip()
{
    return QObject::tr("Paint Widget's content to printer or generate PDF file.");
}

MiscellaneousTestGroup MiscellaneousPrinterWidget::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousPrinterWidget::GetItemID()
{
    return MiscellaneousTestItem::QT_Printer_Widget;
}
