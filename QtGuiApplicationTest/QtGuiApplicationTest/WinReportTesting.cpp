#include "WinReportTesting.h"
#include "ui_WinReportTesting.h"

// QT Headers
#include <QFile>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextTable>
#include <QPrinter>
#include <QPainter>
#include <QTcpSocket>
#include <QThread>
#include <QNetworkInterface>
#include <QDateTime>
#include <QMetaEnum>

// MuPDF
//#include <mupdf/fitz.h>
#include <mupdf/pdf.h>

// Protocol Buffer
#include "MessageInfo.pb.h"

// Self Define
#include <MemUtil.h>
#include <LogUtil.h>
#include "FramelessWindowHelper.h"
#include "XmlReportGenerator.h"
#include "ReportXmlData.h"
#include "FopReportProtocol.h"

using namespace SL::Util;
using namespace com::genomics::protobuf;

WinReportTesting::WinReportTesting(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , ui(new Ui::WinReportTesting())
    , mpFramelessHelper(Q_NULLPTR)
{
    ui->setupUi(this);
    mpFramelessHelper = new FramelessWindowHelper(this);
    mpSocketClient1 = new QTcpSocket();
    mpSocketClient2 = new QTcpSocket();

    this->connect(&mPdfPreview, SIGNAL(SignalErrorOccurred(MuPDF::ErrorCode, QString)), SLOT(SlotMuPdfError(MuPDF::ErrorCode, QString)));
    this->connect(&mFopClient, SIGNAL(SignalSaveCompletely()), SLOT(SlotSaveCompletely()));
}

WinReportTesting::~WinReportTesting()
{
    delete ui;

    QObject::disconnect(&mPdfPreview, SIGNAL(SignalErrorOccurred(MuPDF::ErrorCode, QString)), this, SLOT(SlotMuPdfError(MuPDF::ErrorCode, QString)));
    mPdfPreview.Close();

    MemUtil::RelesePointer(&mpFramelessHelper);
    if (mpSocketClient1->isOpen())
    {
        mpSocketClient1->close();
    }
    delete mpSocketClient1;
    if (mpSocketClient2->isOpen())
    {
        mpSocketClient2->close();
    }
    delete mpSocketClient2;
}

void WinReportTesting::on_btnSavePDF_clicked()
{
    static int filenamePostfix = 0;
    QString xmlFilename = "E:/Temp/FopTest/MGI_ReportTestByQt.xml";
    QString xslFilename = "E:/Temp/FopTest/MGI_ReportTest_Upgrade.xsl";
    QString outFilename = QString("E:/Temp/FopTest/MGI_ReportTest_%1.pdf").arg(++filenamePostfix);
    mFopClient.SavePDF("SAVE", xmlFilename, xslFilename, outFilename);

    return;

    QString outXmlFilename = "E:/Temp/FopTest/MGI_ReportTestByQt.xml";
    XmlReportGenerator xmlReport;
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_PatientInfo_Name, ui->leName->text());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_PatientInfo_Age, ui->leAge->text());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_UltrasoundImages_USImage, ui->leUsImgPath->text());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_PatientInfo_ExamDescription, ui->leExamDescription->toPlainText());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_FOOTER_PrintTime, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    xmlReport.SaveReportAsXml(outXmlFilename);

    QString serverIP = "localhost";
    int serverPort = 8000;

    mpSocketClient1->connectToHost(serverIP, serverPort);
    bool connectFlag1 = mpSocketClient1->waitForConnected(5000);

    int msgBodyLen = 0;
    int msgLen = 0;
    char *pTempMsgBuffer = nullptr;
    int count = 10;
    if (connectFlag1)
    {
        while (count > 0)
        {
            MessageHeader *pMsgHeader = new MessageHeader();
            pMsgHeader->set_msgid(count);
            pMsgHeader->set_version(1);
            pMsgHeader->set_msgtype(MsgType::MsgTypeCommand);
            MessageCommand *pMsgCommand = new MessageCommand();
            pMsgCommand->set_cmd("SAVE");
            //pMsgCommand->set_xmlfilename("E:/Temp/FopTest/MGI_ReportTest.xml");
            pMsgCommand->set_xmlfilename(outXmlFilename.toStdString().c_str());
            pMsgCommand->set_xslfilename("E:/Temp/FopTest/MGI_ReportTest.xsl");
            //pMsgCommand->set_outfilename("E:/Temp/FopTest/QtReportTest.pdf");

            QString saveFilename = QString("E:/Temp/FopTest/MGI_ReportTest_%1.pdf").arg(++filenamePostfix);
            pMsgCommand->set_outfilename(saveFilename.toStdString().c_str());

            MessageBody *pMsgBody = new MessageBody();
            pMsgBody->set_allocated_msgcommand(pMsgCommand);
            MessageInfo msgInfo;
            msgInfo.set_allocated_msgheader(pMsgHeader);
            msgInfo.set_allocated_msgbody(pMsgBody);
            msgBodyLen = msgInfo.ByteSize();
            msgLen = msgBodyLen + 4;
            pTempMsgBuffer = new char[msgLen];
            memcpy(pTempMsgBuffer, &msgLen, 4);
            msgInfo.SerializeToArray(pTempMsgBuffer + 4, msgBodyLen);

            mpSocketClient1->write(pTempMsgBuffer, msgLen);
            mpSocketClient1->waitForBytesWritten();
            //LogUtil::Debug(CODE_LOCATION, "write MsgID: %d", count);
            delete[]pTempMsgBuffer;
            --count;
        }
        //mpSocketClient1->close();
    }
}

void WinReportTesting::on_btnPreviewFOP_clicked()
{
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    for each (QNetworkInterface netInterface in interfaceList)
    {
        QString name = netInterface.name();
        QString hardwareAddr = netInterface.hardwareAddress();
        QList<QNetworkAddressEntry> addrEntryList = netInterface.addressEntries();
        for (int i = 0; i < addrEntryList.count(); ++i)
        {
            QString ip = addrEntryList.at(i).ip().toString();
            QString mask = addrEntryList.at(i).netmask().toString();
            QString broadcast = addrEntryList.at(i).broadcast().toString();
        }
    }

    QString outXmlFilename = "E:/Temp/FopTest/MGI_ReportTestByQt.xml";
    XmlReportGenerator xmlReport;
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_PatientInfo_Name, ui->leName->text());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_PatientInfo_Age, ui->leAge->text());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_UltrasoundImages_USImage, ui->leUsImgPath->text());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_PatientInfo_ExamDescription, ui->leExamDescription->toPlainText());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_FOOTER_PrintTime, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    xmlReport.SaveReportAsXml(outXmlFilename);

    QString serverIP = "localhost";
    int serverPort = 8000;

    mpSocketClient1->connectToHost(serverIP, serverPort);
    bool connectFlag1 = mpSocketClient1->waitForConnected(5000);

    MessageHeader *pMsgHeader = new MessageHeader();
    pMsgHeader->set_msgid(124);
    pMsgHeader->set_version(1);
    pMsgHeader->set_msgtype(MsgType::MsgTypeCommand);
    MessageCommand *pMsgCommand = new MessageCommand();
    pMsgCommand->set_cmd("AWT");
    //pMsgCommand->set_xmlfilename("E:/Temp/FopTest/MGI_ReportTest.xml");
    pMsgCommand->set_xmlfilename(outXmlFilename.toStdString().c_str());
    pMsgCommand->set_xslfilename("E:/Temp/FopTest/MGI_ReportTest.xsl");
    pMsgCommand->set_outfilename("");
    MessageBody *pMsgBody = new MessageBody();
    pMsgBody->set_allocated_msgcommand(pMsgCommand);
    MessageInfo msgInfo;
    msgInfo.set_allocated_msgheader(pMsgHeader);
    msgInfo.set_allocated_msgbody(pMsgBody);

    int msgBodyLen = msgInfo.ByteSize();
    int msgLen = msgBodyLen + 4;
    char *pTempMsgBuffer = new char[msgLen];
    memcpy(pTempMsgBuffer, &msgLen, 4);
    msgInfo.SerializeToArray(pTempMsgBuffer + 4, msgBodyLen);

    if (connectFlag1)
    {
        mpSocketClient1->write(pTempMsgBuffer, msgLen);
        mpSocketClient1->waitForBytesWritten();
        mpSocketClient1->close();
    }

    delete[]pTempMsgBuffer;
}

void WinReportTesting::on_btnPreviewMuPDF_clicked()
{
    char *input = const_cast<char*>("E:/Temp/FopTest/QtReportTest.pdf");
    float zoom, rotate;
    int page_number, page_count;
    fz_context *context = NULL;
    fz_document *document = NULL;
    fz_pixmap *pixmap = NULL;
    fz_rect pageBox;
    fz_matrix ctm;
    page_number = 0;
    zoom = 100;
    rotate = 0;

    context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (context == NULL)
    {
        LogUtil::Error(CODE_LOCATION, "fz_new_contex failed!");
    }
    fz_try(context)
    {
        ::fz_register_document_handlers(context);
        document = ::fz_open_document(context, input);
        page_count = ::fz_count_pages(context, document);
        if (page_number < 0 || page_number >= page_count)
        {
            qDebug() << stderr << "page number out of range: " << page_number + 1 << "page count:" << page_count;
            ::fz_drop_document(context, document);
            ::fz_drop_context(context);
            return;
        }
        pageBox = ::fz_make_rect(0, 0, 100, 100);
        ctm = ::fz_transform_page(pageBox, 96, 0);
        //ctm = ::fz_scale(zoom / 100, zoom / 100);
        ::fz_pre_rotate(ctm, rotate);
        pixmap = ::fz_new_pixmap_from_page_number(context, document, page_number, ctm, fz_device_rgb(context), 0);
    }
    fz_catch(context)
    {
        LogUtil::Debug(CODE_LOCATION, "MuPDF error: %s", ::fz_caught_message(context));
        ::fz_drop_document(context, document);
        ::fz_drop_context(context);
        return;
    }

    unsigned char *samples = pixmap->samples;
    int width = ::fz_pixmap_width(context, pixmap);
    int height = ::fz_pixmap_height(context, pixmap);

    QImage image(samples, width, height, pixmap->stride, QImage::Format_RGB888);
    //ui->lbImgPdfView->setPixmap(
    //    QPixmap::fromImage(
    //    image.scaled(ui->lbImgPdfView->width(), ui->lbImgPdfView->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation)
    //    )
    //);

    ::fz_drop_pixmap(context, pixmap);
    ::fz_drop_document(context, document);
    ::fz_drop_context(context);
}

void WinReportTesting::on_btnPrintImgPDF_clicked()
{
    mPdfPreview.Print();
    return;

    QPrinter::Margins margins = { 0.0f, 0.0f, 0.0f, 0.0f };
    QPrinter imagePrinter(QPrinter::HighResolution);
    imagePrinter.setPageSize(QPagedPaintDevice::A4);
    imagePrinter.setMargins(margins);
    imagePrinter.setPageMargins(0.0, 0.0, 0.0, 0.0, QPrinter::Millimeter);
    //imagePrinter.setFullPage(true);
    //imagePrinter.setOutputFormat(QPrinter::PdfFormat);
    //imagePrinter.setOutputFileName("E:/Temp/FopTest/MGI_ReportTest_image_print.pdf");

    int printerResolution = imagePrinter.resolution();

    char *input = const_cast<char*>("E:/Temp/FopTest/QtReportTest.pdf");
    float zoom, rotate;
    int page_number, page_count;
    fz_context *context = NULL;
    fz_document *document = NULL;
    fz_pixmap *pixmap = NULL;
    fz_rect pageBox;
    fz_matrix ctm;
    page_number = 0;
    zoom = 100;
    rotate = 0;

    context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (context == NULL)
    {
        LogUtil::Error(CODE_LOCATION, "fz_new_contex failed!");
    }
    fz_try(context)
    {
        ::fz_register_document_handlers(context);
        document = ::fz_open_document(context, input);
        page_count = ::fz_count_pages(context, document);
        if (page_number < 0 || page_number >= page_count)
        {
            qDebug() << stderr << "page number out of range: " << page_number + 1 << "page count:" << page_count;
            ::fz_drop_document(context, document);
            ::fz_drop_context(context);
            return;
        }

        QSizeF pageSizeMM = imagePrinter.pageSizeMM();
        pageBox = ::fz_make_rect(0, 0, pageSizeMM.width(), pageSizeMM.height());
        ctm = ::fz_transform_page(pageBox, printerResolution, 0);
        //ctm = ::fz_scale(zoom / 100, zoom / 100);
        //::fz_pre_rotate(ctm, rotate);
        pixmap = ::fz_new_pixmap_from_page_number(context, document, page_number, ctm, fz_device_rgb(context), 0);
    }
    fz_catch(context)
    {
        LogUtil::Debug(CODE_LOCATION, "MuPDF error: %s", ::fz_caught_message(context));
        ::fz_drop_document(context, document);
        ::fz_drop_context(context);
        return;
    }

    unsigned char *samples = pixmap->samples;
    int width = ::fz_pixmap_width(context, pixmap);
    int height = ::fz_pixmap_height(context, pixmap);

    QImage image(samples, width, height, pixmap->stride, QImage::Format_RGB888);
    //image.save("E:/Temp/FopTest/MGI_ReportTest_image_print.png");
    int imgWidth = image.width();
    int imgHeight = image.height();

    QPainter imagePainter;
    bool resFlag = imagePainter.begin(&imagePrinter);
    QRect painterRect = imagePainter.viewport();
    imagePainter.drawImage(painterRect, image);
    resFlag = imagePainter.end();

    ::fz_drop_pixmap(context, pixmap);
    ::fz_drop_document(context, document);
    ::fz_drop_context(context);
}

void WinReportTesting::on_btnMuPdfWrap_clicked()
{
    mPdfPreview.Open(QString("E:/Temp/FopTest/QtReportTest.pdf"));
    //mPdfPreview.Open(QString("E:\\Temp\\FopTest\\mupdf_explored.pdf"));
    QImage firstPage = mPdfPreview.PageFirst();
    //this->ShowPdfImage(firstPage);

    if (!firstPage.isNull())
    {
        ui->lbPdfPreview->setPixmap(
            QPixmap::fromImage(
                firstPage.scaled(ui->lbPdfPreview->width(), ui->lbPdfPreview->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)
            ));
    }
}

void WinReportTesting::on_btnMuPdfWrapPre_clicked()
{
    QImage pdfImage = mPdfPreview.PagePrevious();
    //this->ShowPdfImage(pdfImage);
}

void WinReportTesting::on_btnMuPdfWrapNext_clicked()
{
    QImage pdfImage = mPdfPreview.PageNext();
    //this->ShowPdfImage(pdfImage);
}

void WinReportTesting::on_btnMuPdfRotate_clicked()
{
    static float angleDegree = 0;
    mPdfPreview.PageRotate(angleDegree += 90);
    
    QImage firstPage = mPdfPreview.GetPage(mPdfPreview.PageNumberCurrent());
    if (!firstPage.isNull())
    {
        ui->lbPdfPreview->setPixmap(
            QPixmap::fromImage(
                firstPage.scaled(ui->lbPdfPreview->width(), ui->lbPdfPreview->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)
            ));
    }
}

void WinReportTesting::SlotMuPdfError(MuPDF::ErrorCode code, QString errorString)
{
    QMetaEnum muPdfErrorEnum = QMetaEnum::fromType<MuPDF::ErrorCode>();
    LogUtil::Debug(CODE_LOCATION, "%s : %s", muPdfErrorEnum.valueToKey(code), errorString.toStdString().c_str());
}

void WinReportTesting::SlotSaveCompletely()
{
    LogUtil::Debug(CODE_LOCATION, "Save PDF completely");
}
