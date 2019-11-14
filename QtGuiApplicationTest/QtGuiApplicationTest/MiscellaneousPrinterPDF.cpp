#include "MiscellaneousPrinterPDF.h"

#include <QPainter>
#include <QPrinter>
#include <QPrinterInfo>

#include <LogUtil.h>

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

void MiscellaneousPrinterPDF::on_btnQPrinterTest_clicked()
{
    QList<QPrinterInfo> printerList = QPrinterInfo::availablePrinters();
    for each (QPrinterInfo printerInfo in printerList)
    {
        LogUtil::Debug(CODE_LOCATION, "Printer:%s IsRemote:%s IsDefault:%s",
                       printerInfo.printerName().toUtf8().data(),
                       printerInfo.isRemote() ? "true" : "false",
                       printerInfo.isDefault() ? "true" : "false");
    }

    QPrinter::Margins margins = { 0.0f, 0.0f, 0.0f, 0.0f };
    QPrinter imagePrinter(QPrinter::HighResolution);
    imagePrinter.setPageSize(QPagedPaintDevice::A4);
    imagePrinter.setMargins(margins);
    imagePrinter.setPageMargins(0.0, 0.0, 0.0, 0.0, QPrinter::Millimeter);
    //imagePrinter.setFullPage(true);
    //imagePrinter.setOutputFormat(QPrinter::PdfFormat);
    //imagePrinter.setOutputFileName("E:/Temp/FopTest/MGI_ReportTest_image_print.pdf");

    int printerResolution = imagePrinter.resolution();

    QPainter imagePainter;
    imagePainter.begin(&imagePrinter);
    QRect painterRect = imagePainter.viewport();
    imagePainter.drawText(QPoint(10, 10), "Hello world!");
    imagePainter.end();
}
