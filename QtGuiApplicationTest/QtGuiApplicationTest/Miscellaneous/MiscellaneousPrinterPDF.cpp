#include "MiscellaneousPrinterPDF.h"

#include <QPainter>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QPrintPreviewWidget>
#include <QPrintPreviewDialog>
#include <QPageSetupDialog>

#include <LogUtil.h>

MiscellaneousPrinterPDF::MiscellaneousPrinterPDF(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);

    QPrinter* pPrinter = new QPrinter(QPrinter::HighResolution);
    mpPrintPreviewWgt = new QPrintPreviewWidget(pPrinter, this);
    //mpPrintPreviewWgt->setStyleSheet("background-color:red;"); // 打印预览框背景设置无效：内部采用场景控件，灰色背景刷，没有对外提供接口。
    connect(mpPrintPreviewWgt, &QPrintPreviewWidget::paintRequested, this, &MiscellaneousPrinterPDF::SlotPreviewPaintRequested);
    ui.gridLayout->addWidget(mpPrintPreviewWgt, 3, 1, 1, 1);
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

void MiscellaneousPrinterPDF::SlotPreviewPaintRequested(QPrinter* printer)
{
    static int paintCount = 0;
    ++paintCount;

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
        testStrList << QString("Test string line %1 - %2").arg(i).arg(paintCount);
    }

    int count = testStrList.count();
    for (int i = 0, j = 1; i < count; i++)
    {
        QString text = testStrList[i];
        painter.drawText(10, painter.fontMetrics().height() * j++, text);

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
}

void MiscellaneousPrinterPDF::on_btnPrinterInfo_clicked()
{
    QList<QPrinterInfo> printerList = QPrinterInfo::availablePrinters();
    for (QPrinterInfo printerInfo : printerList)
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

void MiscellaneousPrinterPDF::on_btnPrintPreviewWgt_clicked()
{
    QList<int> intList;
    intList.append(1);
    intList.append(2);
    intList.append(3);

    QList<QString> stringList;
    stringList.append("A");
    stringList.append("B");
    stringList.append("C");

    //static int clickCount = 0;
    //switch (++clickCount % 3)
    //{
    //case 0:
    //    mpPrintPreviewWgt->updatePreview();
    //    break;
    //case 1:
    //    mpPrintPreviewWgt->zoomIn();
    //    break;
    //case 2:
    //    mpPrintPreviewWgt->zoomOut();
    //    break;
    //default:
    //    break;
    //}
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

void MiscellaneousPrinterPDF::on_btnQPrinterCustom_clicked()
{
    QPrinter printer(QPrinter::ScreenResolution);

    // 页面布局相关 —— 尺寸不在合理范围内（Min ~ Max）或者单位不一致，将导致设置边距失败。
    //QPageLayout pageLayout = printer.pageLayout();
    //QPageLayout::Unit pageUnit = pageLayout.units();
    //QMarginsF maxMargins = pageLayout.maximumMargins();
    //QMarginsF minMargins = pageLayout.minimumMargins();
    //bool testFlag1 = pageLayout.setMargins(QMarginsF(20.0, 30.0, 40.0, 50.0));
    //bool testFlag2 = printer.setPageMargins(QMarginsF(20.0, 30.0, 40.0, 50.0), pageUnit);
    //printer.setPageOrientation(QPageLayout::Landscape);
    //printer.setPageSize(QPagedPaintDevice::A4Extra);
    ////QPagedPaintDevice::Margins margins = { 1.0, 2.0, 3.0, 4.0 };
    ////printer.setMargins(margins);

    // 页面布局相关 —— 通过 QPageLayout 设置边距
    //QPageLayout pageLayout(QPageSize(QPageSize::A4),QPageLayout::Landscape, QMarginsF(1.0, 2.0, 3.0, 4.0), QPageLayout::Millimeter, QMarginsF(0.0, 0.0, 0.0, 0.0));
    QPageLayout pageLayout = printer.pageLayout();
    QMarginsF minMargins1 = pageLayout.minimumMargins();
    pageLayout.setMode(QPageLayout::StandardMode);
    pageLayout.setUnits(QPageLayout::Millimeter);
    QMarginsF minMargins2 = pageLayout.minimumMargins();
    pageLayout.setMinimumMargins(QMarginsF(5.0, 5.0, 5.0, 5.0));
    pageLayout.setMargins(QMarginsF(5.0, 5.0, 4.0, 5.0));
    QPageLayout::Unit pageUnit = pageLayout.units();
    QMarginsF maxMargins = pageLayout.maximumMargins();
    QMarginsF minMargins = pageLayout.minimumMargins();
    bool testFlag2 = printer.setPageLayout(pageLayout);

    // 打印机属性相关
    printer.setPrintRange(QPrinter::PageRange);                   // 打印页范围模式：所有页、选中的页、指定范围、当前页。
    printer.setFromTo(1, 2);                                      // 打印页范围
    printer.setCopyCount(5);                                      // 打印份数
    printer.setPrinterName(QString("HP LaserJet Pro MFP M127-M128 PCLmS"));
    printer.setDoubleSidedPrinting(true);                         // 是否翻页
    printer.setDuplex(QPrinter::DuplexShortSide);                 // 长/短边翻页

    QPrinter::PaperSource paperSource = printer.paperSource();
    QList<QPrinter::PaperSource> paperSources = printer.supportedPaperSources();
    QList<int> resolutionList = printer.supportedResolutions();
    bool supportCopyFlag = printer.supportsMultipleCopies();

    QPageSetupDialog pageSetupDlg(&printer);
    if (QDialog::Accepted == pageSetupDlg.exec())
    {
        qreal left = 0.0, top = 0.0, right = 0.0, bottom = 0.0;
        printer.getPageMargins(&left, &top, &right, &bottom, QPrinter::Millimeter);
        printer.getPageMargins(&left, &top, &right, &bottom, QPrinter::Inch);
        int i = 0;
    }
    QPrintDialog printDlg(&printer, this);
    if (QDialog::Accepted == printDlg.exec())
    {
        int copyCount = printer.copyCount();
        this->render(&printer);
    }
}
