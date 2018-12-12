#include "MainTabPageFirst.h"

// QT Headers
#include <QString>
#include <QImage>
#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include <QThread>
#include <QThreadPool>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QTextCodec>
#include <QScreen>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QMetaType>

/* DCMTK 3.6.3 Headers */
#include <dcmtk/ofstd/offile.h>
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcpixseq.h>
#include <dcmtk/dcmdata/dcpxitem.h>
#include <dcmtk/dcmdata/libi2d/i2djpgs.h>
#include <dcmtk/dcmdata/libi2d/i2dbmps.h>

#include <LogUtil.h>
#include <BackgroundWorkerTest.h>
#include <TaskBase.h>

#include "DicomWindow.h"
#include "AppListItemDelegate.h"
#include "MyBackgroundWorker.h"
#include "MyWorkerThreadPool.h"
#include "WorkerTaskBase.h"
#include "MiscellaneousTesting.h"
#include "WinReportTesting.h"
#include "SuspendedScrollBar.h"
#include "UrlUtil.h"

static OFLogger gLogger = OFLog::getLogger("DicomTestLog");

MainTabPageFirst::MainTabPageFirst(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
    , mpBackgroundWorker(Q_NULLPTR)
    , mpMiscellaneousTest(Q_NULLPTR)
    , mpWinReportTest(Q_NULLPTR)
    , mpCurEnteredItem(Q_NULLPTR)
{
    ui.setupUi(this);

    ui.imgTitleValue->setText("image_filename_test");

    pDcmWidget = new DicomWindow();

    mpBackgroundWorker = new SL::Core::BackgroundWorkerTest();
    mpMyWorkerThreadPool = new MyWorkerThreadPool();

    QButtonGroup *pGroupA = new QButtonGroup(this);
    QButtonGroup *pGroupB = new QButtonGroup(this);

    pGroupA->setExclusive(true);
    pGroupA->addButton(ui.aGroupObj1);
    pGroupA->addButton(ui.aGroupObj2);

    pGroupB->setExclusive(true);
    pGroupB->addButton(ui.bGroupObj1);
    pGroupB->addButton(ui.bGroupObj2);
    pGroupB->addButton(ui.bGroupObj3);

    this->InitAppListView();
    //this->InitAppListWidget();
}

MainTabPageFirst::~MainTabPageFirst()
{
    if (mpBackgroundWorker != Q_NULLPTR)
    {
        delete mpBackgroundWorker;
        mpBackgroundWorker = Q_NULLPTR;
    }
    if (mpMiscellaneousTest != Q_NULLPTR)
    {
        delete mpMiscellaneousTest;
    }
    if (mpWinReportTest != Q_NULLPTR)
    {
        delete mpWinReportTest;
    }
}

void MainTabPageFirst::InitAppListView()
{
    ui.lwAppList->setVisible(false);
    ui.lvAppList->setVisible(true);

    //QStringListModel *pStrListModel = new QStringListModel(this);
    //pStrListModel->insertRows(0, 10);
    //for (int i=0; i<10; ++i)
    //{
    //    pStrListModel->setData(pStrListModel->index(i), QString("Item ") + QString('A' + i));
    //}

    mpModelAppListItem = new QStandardItemModel();
    AppListItemData tempAppItemData;
    AppListItemStatus tempAppItemStatus;
    QStandardItem *pTempAppListItem = Q_NULLPTR;

    for (int i = 0; i < 20; ++i)
    {
        tempAppItemData.name = QString("Item %1").arg(i);
        tempAppItemData.description = QString("description string %1").arg(i);
        tempAppItemData.nStar = i;
        pTempAppListItem = new QStandardItem();
        
        int randNum = rand() % 3;
        switch (randNum)
        {
        case 0:
            tempAppItemStatus = ITEMSTATUS_RED;
            break;
        case 1:
            tempAppItemStatus = ITEMSTATUS_BLUE;
            break;
        case 2:
            tempAppItemStatus = ITEMSTATUS_YELLOW;
            break;
        }
        pTempAppListItem->setData(tempAppItemStatus, Qt::UserRole);
        pTempAppListItem->setData(QVariant::fromValue(tempAppItemData), Qt::UserRole + 1);

        mpModelAppListItem->appendRow(pTempAppListItem);
    }

    ui.lvAppList->setItemDelegate(new AppListItemDelegate(this));
    ui.lvAppList->setViewMode(QListView::IconMode);
    ui.lvAppList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.lvAppList->setModel(mpModelAppListItem);
    SuspendedScrollBar *pSuspendScrollBar = new SuspendedScrollBar(ui.lvAppList->verticalScrollBar(), ui.lvAppList);
    //SuspendedScrollBar *pSuspendScrollBar = new SuspendedScrollBar(Q_NULLPTR, ui.lvAppList);
    //pSuspendScrollBar->setRange(0, 300);
    //pSuspendScrollBar->setPageStep(10);
    //this->connect(pSuspendScrollBar, SIGNAL(valueChanged(int)), SLOT(SlotSuspendBarValueChanged(int)));

    // ScrollBar testing
    //QScrollBar *pScrollBar = new QScrollBar(Qt::Horizontal, ui.lvAppList);
    //pScrollBar->setFixedWidth(200);
    //pScrollBar->setValue(0);
    //pScrollBar->setRange(0, 100);
    //pScrollBar->setPageStep(10);
}

void MainTabPageFirst::InitAppListWidget()
{
    ui.lvAppList->setVisible(false);
    ui.lwAppList->setVisible(true);

    QString icon;
    QListWidgetItem *pAppItem = Q_NULLPTR;
    QSize gridSize = ui.lvAppList->gridSize();
    QSize itemHintSize = gridSize - QSize(2, 2);

    ui.lwAppList->clear();
    for (int i = 0; i < 80; ++i)
    {
        pAppItem = new QListWidgetItem();
        icon = UrlUtil::GetAppIcon(i % 4 + 1);
        pAppItem->setText(QString("test_%1").arg(i % 4 + 1));
        pAppItem->setIcon(QIcon(icon));
        pAppItem->setSizeHint(itemHintSize);
        ui.lwAppList->addItem(pAppItem);
    }
    SuspendedScrollBar *pSuspendScrollBar = new SuspendedScrollBar(ui.lwAppList->verticalScrollBar(), ui.lwAppList);
}

bool MainTabPageFirst::event(QEvent *event)
{
    //LogUtil::Debug(CODE_LOCATION, "eventType: %d", event->type());
    switch (event->type())
    {
    case QEvent::Show:
        break;
    default:
        break;
    }
    return QWidget::event(event);
}

void MainTabPageFirst::showEvent(QShowEvent *event)
{
    return QWidget::showEvent(event);
}

void MainTabPageFirst::on_btnBrowserDcm_clicked()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString dcmFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("DICOM (*.dcm)"));
    ShowDicomImage(dcmFileName);
}

void MainTabPageFirst::on_btnBrowserImg_clicked()
{
    //this->SelfPaintImage8Bit();
    //this->ReadJpegAndCopyToDicom();
    this->ReadImageByQImage();
    //this->ReadImageByQImageMulti();
    //this->BackgroundWorkerTest();
    //this->QThreadPoolTest();
    //this->MyThradPoolTest();
}

void MainTabPageFirst::on_btnPrint_clicked()
{
    //QString test1 = QString::fromLocal8Bit("test中文");
    //QString test2 = QStringLiteral("test中文");

    //std::string stdString1 = test1.toStdString();
    //std::string stdString2 = test2.toStdString();

    //return;

    if (mpWinReportTest == Q_NULLPTR)
    {
        mpWinReportTest = new WinReportTesting();
        //mpWinReportTest->setAttribute(Qt::WA_ShowModal, true);
    }
    mpWinReportTest->show();
    return;

    QScreen *primaryScreen = QApplication::primaryScreen();
    QSizeF physicsSizeF = primaryScreen->physicalSize();
    QSize screenSize = primaryScreen->size();

    qreal logicalDotsPerInch = primaryScreen->logicalDotsPerInch();
    qreal logicalDotsPerInchX = primaryScreen->logicalDotsPerInchX();
    qreal logicalDotsPerInchY = primaryScreen->logicalDotsPerInchY();

    qreal physicalDotsPerInch = primaryScreen->physicalDotsPerInch();
    qreal physicalDotsPerInchX = primaryScreen->physicalDotsPerInchX();
    qreal physicalDotsPerInchY = primaryScreen->physicalDotsPerInchY();

    int a4Width = 210 / 25.4 * logicalDotsPerInchX + 0.05;
    int a4Height = 297 / 25.4 * logicalDotsPerInchY + 0.05;

    //return;

    QPrinter  printer(QPrinter::ScreenResolution);
    //printer.setPageSize(QPrinter::A4);
    //printer.setOutputFormat(QPrinter::PdfFormat);
    //printer.setOutputFileName("E:\\Temp\\PrintTest.pdf");
    QRect pageRect = printer.pageRect();
    QPagedPaintDevice::PageSize pageSize = printer.pageSize();

    int printerHeight = printer.height();
    int printerWidth = printer.width();

    QString htmlString =
        "<h1>title1</h1>"
        "<h2>title2</h2>"
        "<table border=1 cellspacing=0 cellpadding=10>"
        "<tr>"
        "<td>1,2</td>"
        "<td>1,1</td>"
        "<td>1,3</td>"
        "</tr>"
        "<tr>"
        "<td>2,1</td>"
        "<td>2,2</td>"
        "<td>2,3</td>"
        "</tr>"
        "<tr>"
        "<td>3,1</td>"
        "<td>3,3</td>"
        "<td>3,2</td>"
        "</tr>"
        "</table>"
        "<table width=595 height=450 border=1 cellspacing=0 cellpadding=10 background=\"file:///E:/Temp/pm.png\">"
        "<tr>"
        "<td><font size=5> TEST ABC </font></td>"
        "</tr>"
        "</table>"
        "<table width=595 height=450 border=1 cellspacing=0 cellpadding=10 background=\"file:///E:/Temp/pm.png\">"
        "<tr>"
        "<td><font size=5> TEST ABC </font></td>"
        "</tr>"
        "</table>";

    //QTextDocument textDocument;
    //textDocument.setHtml(htmlString);
    //textDocument.print(&printer);

    //bool newPageRes = printer.newPage();
    //QTextDocument docNewPage;
    //docNewPage.setHtml("ABC");
    //docNewPage.print(&printer);

    //return;

    //QPrinter printer_html;
    //printer_html.setPageSize(QPrinter::A4);
    //printer_html.setOutputFormat(QPrinter::PdfFormat);
    //printer_html.setOutputFileName("E:\\Temp\\test_html.pdf");

    //QPainter painterTest(&printer_html);
    //QRect viewRect = painterTest.viewport();


    //QStringList title;
    //title.push_back(QStringLiteral("名称"));
    //title.push_back(QStringLiteral("类型"));
    //title.push_back(QStringLiteral("大小"));
    //title.push_back(QStringLiteral("修改日期"));

    //QString curDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    //QString html;
    //html += "<h2 align=\"center\">" + QStringLiteral("HTML导出PDF示例") + "</h2>";
    //html += "<h4 align=\"center\">" + curDateTime + "</h4>";
    //html += "<table width=\"500\" border=\"1\" align=\"center\" style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    //html += "<tr style=\"background-color:gray\">";
    //for (int i = 0; i < title.count(); ++i)
    //{
    //    html += QString("<th>%1</th>").arg(title.at(i));
    //}
    //html += "</tr>";
    //for (int i = 0; i < 120; ++i)
    //{
    //    html += "<tr>";
    //    QString name = QStringLiteral("新建文件夹");
    //    QString type = QStringLiteral("文件夹");
    //    QString size = QStringLiteral("");
    //    if (i % 2)
    //    {
    //        name = QStringLiteral("文本文档");
    //        type = QStringLiteral("文件");
    //        size = QStringLiteral("1KB");
    //    }
    //    html += QString("<td>%1</td>").arg(name);
    //    html += QString("<td>%1</td>").arg(type);
    //    html += QString("<td>%1</td>").arg(size);
    //    html += QString("<td>%1</td>").arg(curDateTime);
    //    html += "</tr>";

    //    if (i>0 && i%10==0)
    //    {
    //        bool newPageFlag = printer_html.newPage();
    //        int i = 0;
    //    }
    //}
    //html += "</table>";

    //QTextDocument text_document;
    //text_document.setHtml(html);
    //text_document.print(&printer_html);
    //bool newPageFlag = printer_html.newPage();
    //text_document.print(&printer_html);

    //return;

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec())
    {
        //QPagedPaintDevice::Margins pdfMargins;
        //pdfMargins.left = 10;
        //pdfMargins.right = 10;
        //pdfMargins.top = 10;
        //pdfMargins.bottom = 10;
        //printer.setMargins(pdfMargins); // 默认单位 毫米 QPageLayout::Millimeter

        //QMarginsF pdfMarginsF(10.0, 10.0, 10.0, 10.0);
        QMarginsF pdfMarginsF(0.0, 0.0, 0.0, 0.0);
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        //printer.setPageMargins(2.65, 2.65, 2.65, 2.65, QPrinter::Unit::Millimeter);
        printer.setPageMargins(pdfMarginsF, QPageLayout::Point);
        printer.setOutputFileName("E:\\Temp\\PrintTest.pdf");

        QString testString = QStringLiteral("Print 测试");
        QPainter painter(&printer);
        QFontMetrics fontMetrics = painter.fontMetrics();
        int textWidth = fontMetrics.width(testString);
        int textHeight = fontMetrics.height();
        QRect painterViewPort = painter.viewport();

        painter.begin(&printer);

        painter.drawText(0, textHeight, testString);

        painter.setPen(QColor(0, 255, 0));
        painter.drawLine(0, 0, textWidth, 0);
        painter.drawLine(textWidth, 0, textWidth, textHeight);
        painter.drawLine(0, textHeight, textWidth, textHeight);
        painter.drawLine(0, 0, 0, textHeight);

        painter.setPen(QColor(0, 255, 255));
        painter.drawLine(0, 0, printerWidth, 0);
        painter.drawLine(printerWidth, 0, printerWidth, printerHeight);
        painter.drawLine(printerWidth, printerHeight-1, 0, printerHeight-1);
        painter.drawLine(0, printerHeight, 0, 0);
        painter.drawLine(0, printerHeight/2, printerWidth, printerHeight / 2);

        if (printer.newPage())
        {
            painter.setPen(QColor(0, 0, 0));
            painter.drawText(0, textHeight, testString);

            painter.setPen(QColor(0, 255, 0));
            painter.drawLine(0, 0, textWidth, 0);
            painter.drawLine(textWidth, 0, textWidth, textHeight);
            painter.drawLine(0, textHeight, textWidth, textHeight);
            painter.drawLine(0, 0, 0, textHeight);

            painter.setPen(QColor(0, 255, 255));
            painter.drawLine(0, 0, printerWidth, 0);
            painter.drawLine(printerWidth, 0, printerWidth, printerHeight);
            painter.drawLine(printerWidth, printerHeight, 0, printerHeight);
            painter.drawLine(0, printerHeight, 0, 0);
        }

        painter.end();
    }
}

void MainTabPageFirst::ShowDicomImage(QString &dcmFileName)
{
    int imgWidth = 512;
    int imgHeight = 512;

    OFFilename dcmFilename(dcmFileName.toStdString().c_str());
    //    OFFilename dcmFilename("e:\\temp\\dicomtesting\\srdoc10\\image12.dcm");
    if (OFStandard::fileExists(dcmFilename))
    {
        OFLOG_DEBUG(gLogger, "Found file: " << dcmFilename);
        DcmFileFormat dcmFileFormat;
        OFCondition loadRes = dcmFileFormat.loadFile(dcmFilename);
        if (loadRes.good())
        {
            OFLOG_DEBUG(gLogger, "Load successfully: " << dcmFilename);
            DcmDataset *pDcmDataSet = dcmFileFormat.getDataset();

            pDcmDataSet->print(std::cout, DCMTypes::PF_shortenLongTagValues);

            E_TransferSyntax xfer = pDcmDataSet->getOriginalXfer();
            OFLOG_DEBUG(gLogger, "E_TransferSyntax " << xfer);

            OFString patientName;
            OFCondition findRes = pDcmDataSet->findAndGetOFString(DCM_PatientName, patientName);
            if (findRes.good())
            {
                OFLOG_DEBUG(gLogger, "PatientName: " << patientName);
                ui.imgTitleValue->setText(QString::fromLocal8Bit(patientName.c_str()));
            }

            unsigned short bitStored = 0;
            pDcmDataSet->findAndGetUint16(DCM_BitsStored, bitStored); //获取像素的位数 bit
            OFLOG_DEBUG(gLogger, "bitStored: " << bitStored);

            OFString photoInterpretation;
            pDcmDataSet->findAndGetOFString(DCM_PhotometricInterpretation, photoInterpretation); //DCM图片的图像模式
            OFLOG_DEBUG(gLogger, "photoInterpretation: " << photoInterpretation);

            unsigned short samplePerPixel = 0;
            pDcmDataSet->findAndGetUint16(DCM_SamplesPerPixel, samplePerPixel);
            OFLOG_DEBUG(gLogger, "samplePerPixel: " << samplePerPixel);

            unsigned short mImgWidth; //获取图像的窗宽高
            unsigned short mImgHeight;
            pDcmDataSet->findAndGetUint16(DCM_Columns, mImgWidth);
            pDcmDataSet->findAndGetUint16(DCM_Rows, mImgHeight);
            OFLOG_DEBUG(gLogger, "imageSize: " << mImgWidth << "x" << mImgHeight);
            imgWidth = mImgWidth;
            imgHeight = mImgHeight;

            const char *pRescaleSlope = Q_NULLPTR;
            const char *pRescaleIntercept = Q_NULLPTR;
            pDcmDataSet->findAndGetString(DCM_RescaleSlope, pRescaleSlope);
            pDcmDataSet->findAndGetString(DCM_RescaleIntercept, pRescaleIntercept);
            int rescaleSlope = pRescaleSlope == Q_NULLPTR ? 0 : atoi(pRescaleSlope);
            int rescaleIntercept = pRescaleIntercept == Q_NULLPTR ? 0 : atoi(pRescaleIntercept);

            const char *pWindowCenter = Q_NULLPTR;
            const char *pWindowWidth = Q_NULLPTR;
            pDcmDataSet->findAndGetString(DCM_WindowCenter, pWindowCenter);
            pDcmDataSet->findAndGetString(DCM_WindowWidth, pWindowWidth);
            int windowCenter = pWindowCenter == Q_NULLPTR ? 0 : atoi(pWindowCenter);
            int windowWidth = pWindowWidth == Q_NULLPTR ? 0 : atoi(pWindowWidth);

            const char* transferSyntax = NULL;
            findRes = dcmFileFormat.getMetaInfo()->findAndGetString(DCM_TransferSyntaxUID, transferSyntax);
            if (findRes.good())
            {
                OFLOG_DEBUG(gLogger, "TransferSyntaxUID: " << transferSyntax);
                if (OFString(transferSyntax) == OFString(UID_JPEGProcess14SV1TransferSyntax)
                    || OFString(transferSyntax) == OFString(UID_JPEGProcess2_4TransferSyntax)
                    || OFString(transferSyntax) == OFString(UID_JPEGProcess14TransferSyntax)
                    || OFString(transferSyntax) == OFString(UID_JPEGProcess1TransferSyntax))
                {
                    DJDecoderRegistration::registerCodecs();
                    pDcmDataSet->chooseRepresentation(EXS_LittleEndianExplicit, NULL); //对压缩的图像像素进行解压
                    DJDecoderRegistration::cleanup();
                }
                else if (OFString(transferSyntax) == OFString(UID_RLELosslessTransferSyntax))
                {
                    DcmRLEDecoderRegistration::registerCodecs();
                    pDcmDataSet->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
                    DcmRLEDecoderRegistration::cleanup();
                }
                else
                {
                    pDcmDataSet->chooseRepresentation(xfer, NULL);
                }
            }
            else
            {
                OFLOG_ERROR(gLogger, "Find DCM_TransferSyntaxUID error: " << findRes.text());
                pDcmDataSet->chooseRepresentation(EXS_Unknown, NULL);
            }
            DcmElement* element = NULL;
            OFCondition result = pDcmDataSet->findAndGetElement(DCM_PixelData, element);
            if (result.bad() || element == NULL)
                return;
            Uint8  *pImgData8 = Q_NULLPTR;
            Uint16 *pImgData16 = Q_NULLPTR;
            Uint32 *pImgData32 = Q_NULLPTR;
            if (8 == bitStored)
            {
                result = element->getUint8Array(pImgData8);
            }
            else if (8 < bitStored && bitStored <= 16)
            {
                result = element->getUint16Array(pImgData16);
            }
            else
            {
                result = element->getUint32Array(pImgData32);
            }

            if (pImgData16 != Q_NULLPTR)
            {
                int minSize = imgWidth < imgHeight ? imgWidth : imgHeight;
                QImage imgTest(imgWidth, imgHeight, QImage::Format_RGB32);
                Sint16 pixelValue = 0;
                int rValue = 0, gValue = 0, bValue = 0;
                const int msgLen = 512 * 10;
                char tempMsg[msgLen] = { 0 };
                char tempPixelValue[10] = { 0 };
                for (int i = 0; i < imgHeight; ++i)
                {
                    ZeroMemory(tempMsg, msgLen);
                    for (int j = 0; j < imgWidth; ++j)
                    {
                        // CT 值的计算公式为： Hu = pixel * slope + intercept
                        pixelValue = pImgData16[i * imgWidth + j] * rescaleSlope + rescaleIntercept;
                        //rValue = pixelValue >> 11;
                        //gValue = pixelValue >> 5 & 0x3F;
                        //bValue = pixelValue & 0x1F;
                        //imgTest.setPixelColor(j, i, QColor(rValue, gValue, bValue, 255));

                        sprintf_s(tempPixelValue, "\t%d", pixelValue);
                        strcat_s(tempMsg, tempPixelValue);

                        if (pixelValue > 200)
                        {
                            pixelValue = 255;
                        }
                        else if (pixelValue < -600)
                        {
                            pixelValue = 0;
                        }
                        else
                        {
                            pixelValue = (pixelValue + 600) * 255.0f / 800;
                        }
                        imgTest.setPixelColor(j, i, QColor(pixelValue, pixelValue, pixelValue, 255));
                    }
                }
                ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));
            }
            else if (pImgData8 != Q_NULLPTR)
            {
                int minSize = imgWidth < imgHeight ? imgWidth : imgHeight;
                QImage imgTest(imgWidth, imgHeight, QImage::Format_RGB32);
                Uint8 pixelValue = 0;
                int *pRgbValue = new int[samplePerPixel];
                for (int i = 0; i < imgHeight; ++i)
                {
                    for (int j = 0; j < imgWidth; ++j)
                    {
                        for (int k = 0; k < samplePerPixel; ++k)
                        {
                            pRgbValue[k] = pImgData8[i * imgWidth * samplePerPixel + j * samplePerPixel + k];
                        }
                        //pixelValue = pImgData8[i * imgWidth + j];
                        imgTest.setPixelColor(j, i, QColor(pRgbValue[0], pRgbValue[1], pRgbValue[2], 255));
                    }
                }
                delete []pRgbValue;
                ui.imgContent->setGeometry(ui.imgContent->x(), ui.imgContent->y(), imgWidth, imgHeight);
                ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));
            }
            else if (pImgData32 != Q_NULLPTR)
            {
            }

        }
        else
        {
            OFLOG_ERROR(gLogger, "Load file failed: " << dcmFilename);

            int minSize = imgWidth < imgHeight ? imgWidth : imgHeight;
            QImage imgTest(imgWidth, imgHeight, QImage::Format_RGB16);
            for (int i = 0; i < imgHeight; ++i)
            {
                for (int j = 0; j < imgWidth; ++j)
                {
                    if (i == j || i + j == minSize || i == imgHeight / 2 || i == imgHeight - 1 || i == 0 || j == 0 || j == imgWidth / 2 || j == imgWidth - 1)
                    {
                        imgTest.setPixelColor(i, j, QColor(255, 0, 0, 255));
                    }
                    else
                    {
                        imgTest.setPixelColor(i, j, QColor(255, 255, 255, 255));
                    }
                }
            }
            ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));
        }
    }
}

void MainTabPageFirst::ReadImageByQImage()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg *.bmp);;All files (*.*)"));

    if (!imgFileName.isEmpty())
    {
        QImage imgTest(imgFileName);

        int width = imgTest.width();
        int height = imgTest.height();
        int oldSamplePerPixel = imgTest.depth() / 8;
        int newSamplePerPixel = oldSamplePerPixel > 3 ? 3 : oldSamplePerPixel; // DICOM 最多 3 通道
        int byteCount = imgTest.byteCount();
        bool allGray = imgTest.allGray();
        const uchar* imgBits = imgTest.constBits();
        QImage::Format imgFormat = imgTest.format();

        ui.imgContent->setPixmap(QPixmap::fromImage(imgTest.scaled(ui.imgContent->size(), Qt::KeepAspectRatio)));

        int indexOfDot = imgFileName.lastIndexOf(".");
        QString saveFilename = imgFileName.left(indexOfDot) + ".dcm";

        OFCondition status;
        DcmFileFormat *pFileFormat = new DcmFileFormat();
        DcmDataset *pDataSet = pFileFormat->getDataset();

        OFString filename = OFString(imgFileName.toStdString().c_str());

        pDataSet->putAndInsertString(DCM_ProtocolName, "Page - Full");

        /* 添加 Patient 信息 */
        pDataSet->putAndInsertString(DCM_AccessionNumber, "MGI001");
        pDataSet->putAndInsertString(DCM_PatientName, "Testing");
        pDataSet->putAndInsertString(DCM_PatientID, "110119");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");
        pDataSet->putAndInsertString(DCM_PatientOrientation, "");
        pDataSet->putAndInsertString(DCM_Laterality, "");
        pDataSet->putAndInsertString(DCM_ImageComments, "");

        /* sop class uid */
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SOPInstanceUID, uid);
        pDataSet->putAndInsertString(DCM_SOPClassUID, UID_UltrasoundImageStorage);

        /* 添加 Study 信息 */
        pDataSet->putAndInsertString(DCM_StudyDate, "20180803");
        pDataSet->putAndInsertString(DCM_ContentDate, "20180803");
        pDataSet->putAndInsertString(DCM_StudyTime, "182230");
        pDataSet->putAndInsertString(DCM_ContentTime, "182230");
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyID, uid);
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

        /* 添加 Series 信息 */
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /* 添加 厂商 信息 */
        pDataSet->putAndInsertString(DCM_Manufacturer, "Nova Microsonics");
        pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "ReferringName");

        /* 添加 Stage 信息 */
        pDataSet->putAndInsertString(DCM_StageName, "StageTest");
        pDataSet->putAndInsertString(DCM_StageNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfStages, "1");
        pDataSet->putAndInsertString(DCM_ViewNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfViewsInStage, "1");

        /* 添加 Image 信息 */
        status = pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
        status = pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        status = pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        status = pDataSet->putAndInsertString(DCM_SeriesNumber, "0");
        status = pDataSet->putAndInsertString(DCM_InstanceNumber, "0");
        status = pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, newSamplePerPixel);
        //status = pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");
        status = pDataSet->putAndInsertString(DCM_Modality, "US");
        status = pDataSet->putAndInsertUint16(DCM_PlanarConfiguration, 0);
        status = pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
        status = pDataSet->putAndInsertUint16(DCM_Rows, height);
        status = pDataSet->putAndInsertUint16(DCM_Columns, width);
        status = pDataSet->putAndInsertUint16(DCM_BitsAllocated, 8);
        status = pDataSet->putAndInsertUint16(DCM_BitsStored, 8);
        status = pDataSet->putAndInsertUint16(DCM_HighBit, 7);
        if (newSamplePerPixel>=3)
        {
            status = pDataSet->putAndInsertOFStringArray(DCM_PhotometricInterpretation, "RGB");
        }
        //status = pDataSet->putAndInsertString(DCM_PixelAspectRatio, "1\\1");
        status = pDataSet->putAndInsertUint16(DCM_PixelRepresentation, 0);


        // 自行组织 像素格式
        int newPixelDataLen = width * height * newSamplePerPixel;
        Uint8 *pImgPixelValues = new Uint8[newPixelDataLen];
        int oldPixelCntPerLine = width * oldSamplePerPixel;
        int newPixelCntPerLine = width * newSamplePerPixel;
        int pixelIndex = 0;
        QRgb tempPixelValue = 0;
        for (int i=0; i<height; ++i)
        {
            for (int j=0; j<width; ++j)
            {
                tempPixelValue = imgTest.pixel(j, i);
                pixelIndex = i*newPixelCntPerLine + j*newSamplePerPixel;
                pImgPixelValues[pixelIndex + 0] = qRed(tempPixelValue);
                pImgPixelValues[pixelIndex + 1] = qGreen(tempPixelValue);
                pImgPixelValues[pixelIndex + 2] = qBlue(tempPixelValue);

                //for (int k = 0; k < newSamplePerPixel; ++k)
                //{
                //    pImgPixelValues[i*newPixelCntPerLine + j*newSamplePerPixel + k] = imgBits[i*oldPixelCntPerLine + j*oldSamplePerPixel + newSamplePerPixel - k - 1];
                //}
            }
        }
        status = pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)pImgPixelValues, newPixelDataLen);
        delete []pImgPixelValues;

        QTextCodec *pGB2312 = QTextCodec::codecForName("System");
        OFFilename newSaveFilename = pGB2312->fromUnicode(saveFilename.toUtf8()).data();

        //status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), EXS_LittleEndianImplicit);
        status = pFileFormat->saveFile(newSaveFilename, EXS_LittleEndianImplicit);
        pDataSet->clear();
        delete pFileFormat;
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
        }
    }
}

void MainTabPageFirst::ReadImageByQImageMulti()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    //QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg *.bmp);;All files (*.*)"));
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), curAppPath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    const int imgCount = 100;

    if (!dir.isEmpty())
    {
        int width = 0, height = 0;
        int samplePerPixel = 1;
        int byteCount = 0;
        const uchar* imgBits = Q_NULLPTR;

        Uint8 *pImgPixelValues[imgCount] = { Q_NULLPTR };
        Uint8 *pAllImageData = Q_NULLPTR;
        QRgb tempPixelValue = 0;
        int byteCountPerLine = 0;
        int pixelIndex = 0;

        QString tempImgName;
        QImage imgTest;
        for (int imgID = 0; imgID < imgCount; ++imgID)
        {
            tempImgName = QString("%1/%2.bmp").arg(dir).arg(imgID+1);
            imgTest.load(tempImgName);

            width = imgTest.width();
            height = imgTest.height();
            samplePerPixel = imgTest.depth() / 8;
            samplePerPixel = samplePerPixel > 3 ? 3 : samplePerPixel; // DICOM 最多 3 通道
            byteCount += imgTest.byteCount();
            imgBits = imgTest.constBits();

            // 自行组织 像素格式
            int newPixelDataLen = width * height * samplePerPixel;
            pImgPixelValues[imgID] = new Uint8[newPixelDataLen];
            byteCountPerLine = width * samplePerPixel;
            pixelIndex = 0;
            tempPixelValue = 0;
            for (int i = 0; i < height; ++i)
            {
                for (int j = 0; j < width; ++j)
                {
                    tempPixelValue = imgTest.pixel(j, i);
                    pixelIndex = i*byteCountPerLine + j*samplePerPixel;
                    pImgPixelValues[imgID][pixelIndex + 0] = qRed(tempPixelValue);
                    pImgPixelValues[imgID][pixelIndex + 1] = qGreen(tempPixelValue);
                    pImgPixelValues[imgID][pixelIndex + 2] = qBlue(tempPixelValue);
                }
            }
        }

        QString saveFilename = dir + "/" + "AllInOne.dcm";

        OFCondition status;
        DcmFileFormat *pFileFormat = new DcmFileFormat();
        DcmDataset *pDataSet = pFileFormat->getDataset();

        pDataSet->putAndInsertString(DCM_ProtocolName, "Page - Full");
        //pDataSet->putAndInsertString(DCM_SpecificCharacterSet, "utf-8");
        pDataSet->putAndInsertString(DCM_SpecificCharacterSet, "ISO-IR 138");

        /* sop class uid */
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SOPInstanceUID, uid);
        pDataSet->putAndInsertString(DCM_SOPClassUID, UID_UltrasoundMultiframeImageStorage);

        /* 添加 Patient 信息 */
        QString test = "test中文";
        QString test1 = QStringLiteral("test中文");

        //QString utf8 = test.toUtf8();
        //QString utf81 = test1.toUtf8();
        //QString utf_8 = QString::fromUtf8(test1.toUtf8());

        char testUtf8[] = { 't','e','s','t', (char)0xE4, (char)0xB8, (char)0xAD, (char)0xE6, (char)0x96, (char)0x87, '\0' };
        pDataSet->putAndInsertString(DCM_AccessionNumber, "MGI001");
        //pDataSet->putAndInsertString(DCM_PatientName, "test中文");
        pDataSet->putAndInsertString(DCM_PatientName, testUtf8);
        pDataSet->putAndInsertString(DCM_PatientID, "110119");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");
        pDataSet->putAndInsertString(DCM_PatientOrientation, test1.toStdString().c_str());
        pDataSet->putAndInsertString(DCM_Laterality, "");
        pDataSet->putAndInsertString(DCM_ImageComments, "test中文");

        QTime curTime = QTime::currentTime();
        QString curTimeString = curTime.toString("hhmmss.zzz");
        QString curDateString = curTime.toString("yyyyMMdd");

        QDateTime curDateTime = QDateTime::currentDateTime();
        QString curTimeStringTest = curDateTime.toString("hhmmss.zzz");
        QString curDateStringTest = curDateTime.toString("yyyyMMdd");

        /* 添加 Study 信息 */
        pDataSet->putAndInsertString(DCM_StudyDate, curDateStringTest.toStdString().c_str());
        pDataSet->putAndInsertString(DCM_ContentDate, curDateStringTest.toStdString().c_str());
        pDataSet->putAndInsertString(DCM_StudyTime, curTimeStringTest.toStdString().c_str());
        pDataSet->putAndInsertString(DCM_ContentTime, curTimeStringTest.toStdString().c_str());
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyID, uid);
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

        /* 添加 Series 信息 */
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /* 添加 厂商 信息 */
        pDataSet->putAndInsertString(DCM_Manufacturer, "Nova Microsonics");
        pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "ReferringName");

        /* 添加 Stage 信息 */
        pDataSet->putAndInsertString(DCM_StageName, "StageTest");
        pDataSet->putAndInsertString(DCM_StageNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfStages, "1");
        pDataSet->putAndInsertString(DCM_ViewNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfViewsInStage, "1");

        /* 添加 Image 信息 */
        status = pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
        status = pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        status = pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        status = pDataSet->putAndInsertString(DCM_SeriesNumber, "0");
        status = pDataSet->putAndInsertString(DCM_InstanceNumber, "0");
        status = pDataSet->putAndInsertString(DCM_Modality, "US");
        status = pDataSet->putAndInsertUint16(DCM_PlanarConfiguration, 0);
        status = pDataSet->putAndInsertString(DCM_NumberOfFrames, QString("%1").arg(imgCount).toStdString().c_str());
        status = pDataSet->putAndInsertUint16(DCM_BitsAllocated, 8);
        status = pDataSet->putAndInsertUint16(DCM_BitsStored, 8);
        status = pDataSet->putAndInsertUint16(DCM_HighBit, 7);

        status = pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, samplePerPixel);
        status = pDataSet->putAndInsertUint16(DCM_Rows, height);
        status = pDataSet->putAndInsertUint16(DCM_Columns, width);

        if (samplePerPixel >= 3)
        {
            status = pDataSet->putAndInsertOFStringArray(DCM_PhotometricInterpretation, "RGB");
        }
        status = pDataSet->putAndInsertUint16(DCM_PixelRepresentation, 0);

        int bytesPerImage = width * height * samplePerPixel;
        pAllImageData = new Uint8[bytesPerImage*imgCount];
        Uint8 *pTempImageData = pAllImageData;
        for (int imgID=0; imgID<imgCount; ++imgID)
        {
            if (pImgPixelValues[imgID] != Q_NULLPTR)
            {
                memcpy_s(pTempImageData, bytesPerImage, pImgPixelValues[imgID], bytesPerImage);
                pTempImageData += bytesPerImage;
                delete[]pImgPixelValues[imgID];
                pImgPixelValues[imgID] = Q_NULLPTR;
            }
        }
        status = pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)pAllImageData, bytesPerImage*imgCount);
        delete[]pAllImageData;

        status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), EXS_LittleEndianImplicit);
        pDataSet->clear();
        delete pFileFormat;
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
        }
    }
}

void MainTabPageFirst::ReadJpegAndCopyToDicom()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg *.bmp);;All files (*.*)"));

    if (!imgFileName.isEmpty())
    {
        QImage imgTest(imgFileName);

        int width = imgTest.width();
        int height = imgTest.height();
        int depth = imgTest.depth();
        int byteCount = imgTest.byteCount();
        bool allGray = imgTest.allGray();
        const uchar* imgBits = imgTest.constBits();
        QImage::Format imgFormat = imgTest.format();

        ui.imgContent->setPixmap(QPixmap::fromImage(imgTest.scaled(ui.imgContent->size(), Qt::KeepAspectRatio)));

        int indexOfDot = imgFileName.lastIndexOf(".");
        QString saveFilename = imgFileName.left(indexOfDot) + ".dcm";

        OFCondition status;
        DcmFileFormat *pFileFormat = new DcmFileFormat();
        DcmDataset *pDataSet = pFileFormat->getDataset();

        OFString filename = OFString(imgFileName.toStdString().c_str());

        I2DImgSource *pImgSource = Q_NULLPTR;
        if (imgFileName.endsWith(".jpg", Qt::CaseInsensitive))
        {
            pImgSource = new I2DJpegSource();
        }
        else if (imgFileName.endsWith(".bmp", Qt::CaseInsensitive))
        {
            pImgSource = new I2DBmpSource();
        }
        if (pImgSource != Q_NULLPTR)
        {
            pImgSource->setImageFile(filename);
            char* pixData = NULL;
            Uint16 rows, cols, samplePerPixel, bitsAlloc, bitsStored, highBit, pixelRpr, planConf, pixAspectH, pixAspectV;
            OFString photoMetrInt;
            Uint32 length;
            E_TransferSyntax ts = EXS_Unknown;
            status = pImgSource->readPixelData(rows, cols, samplePerPixel, photoMetrInt, bitsAlloc, bitsStored, highBit, pixelRpr, planConf, pixAspectH, pixAspectV, pixData, length, ts);
            if (status.bad())
            {
                LogUtil::Error(CODE_LOCATION, "read pixel failed: %s", status.text());
            }

            pDataSet->putAndInsertString(DCM_ProtocolName, "Page - Full");

            /* 添加 Patient 信息 */
            pDataSet->putAndInsertString(DCM_AccessionNumber, "MGI001");
            pDataSet->putAndInsertString(DCM_PatientName, "Testing");
            pDataSet->putAndInsertString(DCM_PatientID, "110119");
            pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
            pDataSet->putAndInsertString(DCM_PatientSex, "M");
            pDataSet->putAndInsertString(DCM_PatientOrientation, "");
            pDataSet->putAndInsertString(DCM_Laterality, "");
            pDataSet->putAndInsertString(DCM_ImageComments, "");

            /* 添加 Study 信息 */
            pDataSet->putAndInsertString(DCM_StudyDate, "20180803");
            pDataSet->putAndInsertString(DCM_ContentDate, "20180803");
            pDataSet->putAndInsertString(DCM_StudyTime, "182230");
            pDataSet->putAndInsertString(DCM_ContentTime, "182230");
            char uid[100];
            dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
            pDataSet->putAndInsertString(DCM_StudyID, uid);
            dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
            pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

            /* 添加 Series 信息 */
            pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
            pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
            memset(uid, 0, sizeof(char) * 100);
            dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
            pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

            /* 添加 厂商 信息 */
            pDataSet->putAndInsertString(DCM_Manufacturer, "Nova Microsonics");
            pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "ReferringName");

            /* 添加 Stage 信息 */
            pDataSet->putAndInsertString(DCM_StageName, "StageTest");
            pDataSet->putAndInsertString(DCM_StageNumber, "1");
            pDataSet->putAndInsertString(DCM_NumberOfStages, "1");
            pDataSet->putAndInsertString(DCM_ViewNumber, "1");
            pDataSet->putAndInsertString(DCM_NumberOfViewsInStage, "1");

            /* 添加 Image 信息 */
            status = pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
            status = pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
            status = pDataSet->putAndInsertString(DCM_ContentTime, "200700");
            status = pDataSet->putAndInsertString(DCM_SeriesNumber, "0");
            status = pDataSet->putAndInsertString(DCM_InstanceNumber, "0");
            status = pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, samplePerPixel);
            //status = pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");
            status = pDataSet->putAndInsertString(DCM_Modality, "US");
            status = pDataSet->putAndInsertUint16(DCM_PlanarConfiguration, 0);
            status = pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
            status = pDataSet->putAndInsertUint16(DCM_Rows, rows);
            status = pDataSet->putAndInsertUint16(DCM_Columns, cols);
            status = pDataSet->putAndInsertUint16(DCM_BitsAllocated, bitsAlloc);
            status = pDataSet->putAndInsertUint16(DCM_BitsStored, bitsStored);
            status = pDataSet->putAndInsertUint16(DCM_HighBit, highBit);
            status = pDataSet->putAndInsertOFStringArray(DCM_PhotometricInterpretation, photoMetrInt);
            //status = pDataSet->putAndInsertString(DCM_PixelAspectRatio, "1\\1");
            status = pDataSet->putAndInsertUint16(DCM_PixelRepresentation, 0);
            status = pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)pixData, length);

            status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), ts);
            pDataSet->clear();
            if (status.bad())
            {
                LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
            }

            delete pImgSource;
        }

        delete pFileFormat;
    }
}

void MainTabPageFirst::BackupFunction()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg)"));

    if (!imgFileName.isEmpty())
    {
        QImage imgTest(imgFileName);

        int width = imgTest.width();
        int height = imgTest.height();
        int depth = imgTest.depth();
        int byteCount = imgTest.byteCount();
        bool allGray = imgTest.allGray();
        const uchar* imgBits = imgTest.constBits();
        QImage::Format imgFormat = imgTest.format();

        ui.imgContent->setPixmap(QPixmap::fromImage(imgTest.scaled(ui.imgContent->size(), Qt::KeepAspectRatio)));

        int indexOfDot = imgFileName.lastIndexOf(".");
        QString saveFilename = imgFileName.left(indexOfDot) + ".dcm";

        DcmFileFormat *pFileFormat = new DcmFileFormat();
        DcmDataset *pDataSet = pFileFormat->getDataset();
        //DcmPixelSequence *pDcmPixelSeq = new DcmPixelSequence(DcmTag(DCM_PixelData, EVR_OB));
        DcmPixelSequence *pDcmPixelSeq = new DcmPixelSequence(DCM_PixelSequenceTag);

        Uint32 dataLength = 0;
        OFCondition status;

        OFString filename = OFString(imgFileName.toStdString().c_str());
        I2DJpegSource* bmpSource = new I2DJpegSource();
        bmpSource->setImageFile(filename);

        char* pixData = NULL;
        Uint16 rows, cols, samplePerPixel, bitsAlloc, bitsStored, highBit, pixelRpr, planConf, pixAspectH, pixAspectV;
        OFString photoMetrInt;
        Uint32 length;
        E_TransferSyntax ts = EXS_Unknown;
        status = bmpSource->readPixelData(rows, cols, samplePerPixel, photoMetrInt, bitsAlloc, bitsStored, highBit, pixelRpr, planConf, pixAspectH, pixAspectV, pixData, length, ts);
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "read pixel failed: %s", status.text());
        }

        DcmXfer transport(ts);
        if (transport.isEncapsulated())
        {
            LogUtil::Debug(CODE_LOCATION, "encapsulated");
        }
        else
        {
            LogUtil::Debug(CODE_LOCATION, "is not encapsulated");
        }

        //DcmPixelItem *newItem = new DcmPixelItem(DcmTag(DCM_Item, EVR_OB));
        DcmPixelItem *newItem = new DcmPixelItem(DCM_PixelItemTag);
        if (newItem != NULL)
        {
            pDcmPixelSeq->insert(newItem);
            status = newItem->putUint8Array((Uint8*)pixData, length);
            if (status.bad())
            {
                LogUtil::Error(CODE_LOCATION, "put pixel data failed: %s", status.text());
            }
        }

        pDataSet->putAndInsertUint16(DCM_AccessionNumber, 0);
        pDataSet->putAndInsertString(DCM_PatientName, "zssure", true);
        pDataSet->putAndInsertString(DCM_PatientID, "2234");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20141221");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");

        /*	添加Study信息	*/
        pDataSet->putAndInsertString(DCM_StudyDate, "20141221");
        pDataSet->putAndInsertString(DCM_StudyTime, "195411");
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);
        pDataSet->putAndInsertString(DCM_StudyID, "1111");


        /*	添加Series信息	*/
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /*	添加Image信息	*/
        pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
        pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        pDataSet->putAndInsertString(DCM_InstanceNumber, "1");
        pDataSet->putAndInsertString(DCM_SamplesPerPixel, "1");
        pDataSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");

        pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, samplePerPixel);
        pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
        pDataSet->putAndInsertUint16(DCM_Rows, rows);
        pDataSet->putAndInsertUint16(DCM_Columns, cols);
        pDataSet->putAndInsertUint16(DCM_BitsAllocated, bitsAlloc);
        pDataSet->putAndInsertUint16(DCM_BitsStored, bitsStored);
        pDataSet->putAndInsertUint16(DCM_HighBit, highBit);
        pDataSet->putAndInsertOFStringArray(DCM_PhotometricInterpretation, photoMetrInt);
        pDataSet->insert(pDcmPixelSeq, OFFalse, OFFalse);

        status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), ts);
        pDataSet->clear();
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
        }

        delete bmpSource;
        delete pFileFormat;
    }
}

void MainTabPageFirst::SelfPaintImage8Bit()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg)"));

    if (!imgFileName.isEmpty())
    {
        QImage imgTest(imgFileName);

        int width = imgTest.width();
        int height = imgTest.height();
        int depth = imgTest.depth();
        int byteCount = imgTest.byteCount();
        bool allGray = imgTest.allGray();
        const uchar* imgBits = imgTest.constBits();
        QImage::Format imgFormat = imgTest.format();

        ui.imgContent->setPixmap(QPixmap::fromImage(imgTest.scaled(ui.imgContent->size(), Qt::KeepAspectRatio)));

        int indexOfDot = imgFileName.lastIndexOf(".");
        QString saveFilename = imgFileName.left(indexOfDot) + ".dcm";

        OFCondition status;
        DcmFileFormat *pFileFormat = new DcmFileFormat();
        DcmDataset *pDataSet = pFileFormat->getDataset();

        const int constWidth = 50;
        const int constHeight = 50;
        Uint8 testImgBuf[constWidth*constHeight] = { 0 };
        for (int i = 0; i < constHeight; ++i)
        {
            for (int j = 0; j < constWidth; ++j)
            {
                if (i == j)
                {
                    testImgBuf[i*constWidth + j] = 0;
                }
                else
                {
                    testImgBuf[i*constWidth + j] = 255;
                }
            }
        }

        pDataSet->putAndInsertString(DCM_ProtocolName, "Page - Full");

        /* 添加 Patient 信息 */
        pDataSet->putAndInsertString(DCM_AccessionNumber, "MGI001");
        pDataSet->putAndInsertString(DCM_PatientName, "Testing");
        pDataSet->putAndInsertString(DCM_PatientID, "110119");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");
        pDataSet->putAndInsertString(DCM_PatientOrientation, "");
        pDataSet->putAndInsertString(DCM_Laterality, "");
        pDataSet->putAndInsertString(DCM_ImageComments, "");

        /* 添加 Study 信息 */
        pDataSet->putAndInsertString(DCM_StudyDate, "20180803");
        pDataSet->putAndInsertString(DCM_ContentDate, "20180803");
        pDataSet->putAndInsertString(DCM_StudyTime, "182230");
        pDataSet->putAndInsertString(DCM_ContentTime, "182230");
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyID, uid);
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

        /* 添加 Series 信息 */
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /* 添加 厂商 信息 */
        pDataSet->putAndInsertString(DCM_Manufacturer, "Nova Microsonics");
        pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "ReferringName");

        /* 添加 Stage 信息 */
        pDataSet->putAndInsertString(DCM_StageName, "StageTest");
        pDataSet->putAndInsertString(DCM_StageNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfStages, "1");
        pDataSet->putAndInsertString(DCM_ViewNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfViewsInStage, "1");

        /* 添加 Image 信息 */
        pDataSet->putAndInsertString(DCM_ImageType, "");
        pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        pDataSet->putAndInsertString(DCM_SeriesNumber, "0");
        pDataSet->putAndInsertString(DCM_InstanceNumber, "0");
        pDataSet->putAndInsertString(DCM_SamplesPerPixel, "1");
        pDataSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        //pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");
        pDataSet->putAndInsertString(DCM_Modality, "US");
        pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, 1);
        //pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
        pDataSet->putAndInsertUint16(DCM_PlanarConfiguration, 0);
        pDataSet->putAndInsertUint16(DCM_Rows, constHeight);
        pDataSet->putAndInsertUint16(DCM_Columns, constWidth);
        pDataSet->putAndInsertUint16(DCM_BitsAllocated, 8);
        pDataSet->putAndInsertUint16(DCM_BitsStored, 8);
        pDataSet->putAndInsertUint16(DCM_HighBit, 7);
        pDataSet->putAndInsertString(DCM_PixelAspectRatio, "1\\1");
        pDataSet->putAndInsertUint16(DCM_PixelRepresentation, 0);
        pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)testImgBuf, constWidth*constHeight);

        /* 配合 MicroDICOM */
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        //pDataSet->putAndInsertString(DCM_SOPClassUID)

        status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), EXS_LittleEndianImplicit, EET_UndefinedLength, EGL_recalcGL, EPD_noChange, 0, 0, EWM_dataset);
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
        }

        pDataSet->clear();
        delete pFileFormat;
    }
}

void MainTabPageFirst::BackgroundWorkerTest()
{
    std::shared_ptr<SL::Core::TaskBase> p = std::shared_ptr<SL::Core::TaskBase>(new SL::Core::TaskBase());
    mpBackgroundWorker->AddTask(p);
    mpBackgroundWorker->AddTask(std::make_shared<SL::Core::TaskBase>());
    mpBackgroundWorker->AddTask(std::make_shared<SL::Core::TaskBase>());
    mpBackgroundWorker->AddTask(std::make_shared<SL::Core::TaskBase>());
    mpBackgroundWorker->AddTask(std::make_shared<SL::Core::TaskBase>());
    mpBackgroundWorker->AddTask(std::make_shared<SL::Core::TaskBase>());
    mpBackgroundWorker->AddTask(std::make_shared<SL::Core::TaskBase>());
    mpBackgroundWorker->AddTask(std::make_shared<SL::Core::TaskBase>());
    mpBackgroundWorker->Start();
}

void MainTabPageFirst::QThreadPoolTest()
{
    MyBackgroundWorker *pTestWorker = new MyBackgroundWorker();
    pTestWorker->setAutoDelete(true);

    QThreadPool::globalInstance()->start(pTestWorker);
    QThreadPool::globalInstance()->start(new MyBackgroundWorker());
    QThreadPool::globalInstance()->start(new MyBackgroundWorker());
    QThreadPool::globalInstance()->start(new MyBackgroundWorker());
    QThreadPool::globalInstance()->start(new MyBackgroundWorker());
    QThreadPool::globalInstance()->start(new MyBackgroundWorker());
    QThreadPool::globalInstance()->start(new MyBackgroundWorker());
    QThreadPool::globalInstance()->start(new MyBackgroundWorker());
    QThreadPool::globalInstance()->start(new MyBackgroundWorker());

    int numOfCpuCore = QThread::idealThreadCount();
    int maxThreadCount = QThreadPool::globalInstance()->maxThreadCount();
    int activeThreadCount = QThreadPool::globalInstance()->activeThreadCount();

    LogUtil::Debug(CODE_LOCATION, "CpuNum:%d MaxThread:%d ActiveThread:%d",
                   numOfCpuCore, maxThreadCount, activeThreadCount);
}

void MainTabPageFirst::MyThradPoolTest()
{
    mpMyWorkerThreadPool->AddTask(SPWorkerTask(new WorkerTaskBase()));
    mpMyWorkerThreadPool->AddTask(SPWorkerTask(new WorkerTaskBase()));
    mpMyWorkerThreadPool->AddTask(SPWorkerTask(new WorkerTaskBase()));
    mpMyWorkerThreadPool->AddTask(SPWorkerTask(new WorkerTaskBase()));
    mpMyWorkerThreadPool->AddTask(SPWorkerTask(new WorkerTaskBase()));
    mpMyWorkerThreadPool->AddTask(SPWorkerTask(new WorkerTaskBase()));
    mpMyWorkerThreadPool->AddTask(SPWorkerTask(new WorkerTaskBase()));
    mpMyWorkerThreadPool->AddTask(SPWorkerTask(new WorkerTaskBase()));
    mpMyWorkerThreadPool->Start();
}
