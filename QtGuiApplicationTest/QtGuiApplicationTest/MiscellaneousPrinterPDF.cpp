#include "MiscellaneousPrinterPDF.h"

MiscellaneousPrinterPDF::MiscellaneousPrinterPDF(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousPrinterPDF::~MiscellaneousPrinterPDF()
{
    ;
}

QString MiscellaneousPrinterPDF::GetTitle()
{
    return QObject::tr("PDF Printer");
}

QString MiscellaneousPrinterPDF::GetTitleTooltip()
{
    return QObject::tr("Paint something to printer or generate PDF file.");
}

MiscellaneousTestGroup MiscellaneousPrinterPDF::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Printer;
}

MiscellaneousTestItem MiscellaneousPrinterPDF::GetItemID()
{
    return MiscellaneousTestItem::Printer_PDF;
}
