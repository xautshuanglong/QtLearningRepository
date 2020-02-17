#include "MiscellaneousPrinterPDF.h"

#include <QPainter>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPageSetupDialog>

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
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousPrinterPDF::GetItemID()
{
    return MiscellaneousTestItem::QT_Printer_PDF;
}

void MiscellaneousPrinterPDF::on_btnPrinterInfo_clicked()
{
    QList<QPrinterInfo> printerList = QPrinterInfo::availablePrinters();
    for each (QPrinterInfo printerInfo in printerList)
    {
        LogUtil::Debug(CODE_LOCATION, "Printer:%s IsRemote:%s IsDefault:%s",
                       printerInfo.printerName().toUtf8().data(),
                       printerInfo.isRemote() ? "true" : "false",
                       printerInfo.isDefault() ? "true" : "false");
    }

    //for (const QPrinterInfo& printerInfo : list)
    //{
    //    info = "defaultDuplexMode: ";
    //    switch (printerInfo.defaultDuplexMode())
    //    {
    //    case QPrinter::DuplexNone:
    //        info += "DuplexNone";
    //        break;
    //    case QPrinter::DuplexAuto:
    //        info += "DuplexAuto";
    //        break;
    //    case QPrinter::DuplexLongSide:
    //        info += "DuplexLongSide";
    //        break;
    //    case QPrinter::DuplexShortSide:
    //        info += "DuplexShortSide";
    //        break;
    //    }
    //    ui->listWidget->addItem(info);

    //    info = "defaultPageSize: ";
    //    info += printerInfo.defaultPageSize().name();
    //    ui->listWidget->addItem(info);

    //    info = "description: ";
    //    info += printerInfo.description();
    //    ui->listWidget->addItem(info);

    //    info = "isDefault: ";
    //    info += printerInfo.isDefault() ? "true" : "false";
    //    ui->listWidget->addItem(info);

    //    info = "isRemote: ";
    //    info += printerInfo.isRemote() ? "true" : "false";
    //    ui->listWidget->addItem(info);

    //    info = "location: " + printerInfo.location();
    //    ui->listWidget->addItem(info);

    //    info = "makeAndModel: " + printerInfo.makeAndModel();
    //    ui->listWidget->addItem(info);

    //    info = "maximumPhysicalPageSize: " + printerInfo.maximumPhysicalPageSize().name();
    //    ui->listWidget->addItem(info);

    //    info = "minimumPhysicalPageSize: " + printerInfo.minimumPhysicalPageSize().name();
    //    ui->listWidget->addItem(info);

    //    info = "printerName: " + printerInfo.printerName();
    //    ui->listWidget->addItem(info);

    //    info = "state: ";
    //    switch (printerInfo.state())
    //    {
    //    case QPrinter::Idle:
    //        info += "Idle";
    //        break;
    //    case QPrinter::Active:
    //        info += "Active";
    //        break;
    //    case QPrinter::Aborted:
    //        info += "Aborted";
    //        break;
    //    case QPrinter::Error:
    //        info += "Error";
    //        break;
    //    }
    //    ui->listWidget->addItem(info);

    //    info = "supportedDuplexModes: ";
    //    for (QPrinter::DuplexMode mode : printerInfo.supportedDuplexModes())
    //    {
    //        switch (mode)
    //        {
    //        case QPrinter::DuplexNone:
    //            info += "DuplexNone";
    //            break;
    //        case QPrinter::DuplexAuto:
    //            info += "DuplexAuto";
    //            break;
    //        case QPrinter::DuplexLongSide:
    //            info += "DuplexLongSide";
    //            break;
    //        case QPrinter::DuplexShortSide:
    //            info += "DuplexShortSide";
    //            break;
    //        }
    //    }
    //    ui->listWidget->addItem(info);

    //    info = "supportedPageSizes: ";
    //    for (QPageSize ps : printerInfo.supportedPageSizes())
    //    {
    //        info += ps.name();
    //    }
    //    ui->listWidget->addItem(info);

    //    info = "supportedResolutions: ";
    //    for (int resolution : printerInfo.supportedResolutions())
    //    {
    //        info += QString::number(resolution);
    //    }
    //    ui->listWidget->addItem(info);

    //    info = "supportsCustomPageSizes: ";
    //    info += printerInfo.supportsCustomPageSizes() ? "true" : false;
    //    ui->listWidget->addItem(info);

    //    ui->listWidget->addItem("");
    //}
}

void MiscellaneousPrinterPDF::on_btnPageSetupDlg_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    QPageSetupDialog dlg(&printer);
    if (QDialog::Accepted == dlg.exec())
    {
        if (printer.pageLayout().orientation() == QPageLayout::Landscape)
        {
            printer.setPageOrientation(QPageLayout::Landscape);
        }
        else
        {
            printer.setPageOrientation(QPageLayout::Portrait);
        }

        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName("E:/Temp/QtPrintTest/PageSetupTest.pdf");
        QPainter painter;
        if (!painter.begin(&printer))
        {
            // failed to open file
            qWarning("failed to open file, is it writable?");
            return;
        }

        QStringList testStrList;
        for (int i = 0; i<200; ++i)
        {
            testStrList << QString("Test string line %1").arg(i);
        }

        int count = testStrList.count();
        for (int i = 0, j = 1; i < count; i++)
        {
            QString text = testStrList[i];
            painter.drawText(10, painter.fontMetrics().height()*j++, text);

            if((i + 1) % 93 == 0)
            {
                if (!printer.newPage())
                {
                    LogUtil::Warn(CODE_LOCATION, "Failed in flushing page to disk, disk full?");
                    break;
                }
                j = 1;
            }
        }
        painter.end();
    }
}

void MiscellaneousPrinterPDF::on_btnPrintPreviewDlg_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog printPreDlg(&printer, this);
    connect(&printPreDlg, &QPrintPreviewDialog::paintRequested, [this](QPrinter* printer)
    {
        QPainter painter;
        if (!painter.begin(printer))
        {
            // failed to open file
            qWarning("failed to open file, is it writable?");
            return;
        }

        QStringList testStrList;
        for (int i = 0; i < 200; ++i)
        {
            testStrList << QString("Test string line %1").arg(i);
        }

        int count = testStrList.count();
        for (int i = 0, j = 1; i < count; i++)
        {
            QString text = testStrList[i];
            painter.drawText(10, painter.fontMetrics().height()*j++, text);

            if ((i + 1) % 93 == 0)
            {
                if (!printer->newPage())
                {
                    LogUtil::Warn(CODE_LOCATION, "Failed in flushing page to disk, disk full?");
                    break;
                }
                j = 1;
            }
        }
        painter.end();
    });
    printPreDlg.exec();
}

void MiscellaneousPrinterPDF::on_btnPrintDlg_clicked()
{
    QStringList testStrList;
    for (int i = 0; i < 200; ++i)
    {
        testStrList << QString("Test string line %1").arg(i);
    }

    QPrinter printer;
    //printer.setFromTo(1, testStrList.size() / 93);
    QPrintDialog printDlg(&printer, this);
    if (QDialog::Accepted == printDlg.exec())
    {
        QPainter painter;

        if (!painter.begin(&printer))
        {
            // failed to open file
            qWarning("failed to open file, is it writable?");
            return;
        }

        int count = testStrList.count();
        for (int i = 0, j = 1; i < count; i++)
        {
            QString text = testStrList[i];
            painter.drawText(10, painter.fontMetrics().height()*j++, text);

            if ((i + 1) % 93 == 0)
            {
                if (!printer.newPage())
                {
                    LogUtil::Warn(CODE_LOCATION, "Failed in flushing page to disk, disk full?");
                    break;
                }
                j = 1;
            }
        }

        bool resFlag = painter.end();
        LogUtil::Info(CODE_LOCATION, "Print %s!", resFlag ? "successfully" : "failed");
    }
}

void MiscellaneousPrinterPDF::on_btnQPrinterTest_clicked()
{
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
    imagePainter.drawText(QPoint(100, 100), "Hello world!");
    imagePainter.end();
}
