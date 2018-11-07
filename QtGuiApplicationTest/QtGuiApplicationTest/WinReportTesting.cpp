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

    ui->teReport->setReadOnly(true);
}

WinReportTesting::~WinReportTesting()
{
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

void WinReportTesting::QTextDocumentTest()
{
    QTextDocument qDoc;
    QTextFrame *pDocFrame = qDoc.rootFrame();
}

void WinReportTesting::QTextEditDocumentTest()
{
    QTextDocument *pEditDoc = ui->teReport->document();
    QTextFrame *pDocFrame = pEditDoc->rootFrame();
    
    QString blockText;
    for (QTextFrame::iterator it=pDocFrame->begin(); it!=pDocFrame->end(); ++it)
    {
        blockText = it.currentBlock().text();
    }

    QTextCursor firstCursor = pDocFrame->firstCursorPosition();
    QTextCursor lastCursor = pDocFrame->lastCursorPosition();

    QTextCursor editCursor = ui->teReport->textCursor();
    QString editCursotStr = editCursor.block().text();

    //QTextDocument *pTempDoc = new QTextDocument(this);
    //QTextFrame *pTempFrame = pTempDoc->rootFrame();
    //QTextBlock firstBlock = pTempDoc->firstBlock();
    //QTextCursor tempFirstCursor = pTempFrame->firstCursorPosition();
    //tempFirstCursor.insertText("line 1");
    //tempFirstCursor.insertText("line 2");
    //tempFirstCursor.insertText("line 3");
    //tempFirstCursor.insertText("line 4");
    //tempFirstCursor.insertBlock();
    //tempFirstCursor.insertText("line 5");
    //tempFirstCursor.insertText("line 6");
    //tempFirstCursor.insertBlock();
    //tempFirstCursor.insertText("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbcccccccccccccccddddddddddddddddeeeeeeeeeee");
    //tempFirstCursor.insertBlock();
    //tempFirstCursor.insertText("a\nb\nc\nd");
    //ui->teReport->setDocument(pTempDoc);
}

void WinReportTesting::on_btnPrintTest_clicked()
{
    //this->QTextDocumentTest();
    //this->QTextEditDocumentTest();
}

void WinReportTesting::on_btnFormatTest_clicked()
{
    QTextDocument *pEditDoc = ui->teReport->document();
    QTextFrame *pDocFrame = pEditDoc->rootFrame();
    QTextFrameFormat format = pDocFrame->frameFormat();
    format.setBorderBrush(Qt::green);
    format.setBorder(1);
    pDocFrame->setFrameFormat(format);

    // QTextCursor 插入测试
    QTextCursor editCursor = ui->teReport->textCursor();
    editCursor.insertBlock();
    QTextCharFormat testCharFormat;
    testCharFormat.setForeground(Qt::blue);
    testCharFormat.setFont(QFont(QStringLiteral("华文行楷"), 20, QFont::Bold));
    editCursor.insertText(QStringLiteral("插入测试"), testCharFormat);

    QTextBlockFormat testBlockFormat;
    testBlockFormat.setBackground(Qt::gray);
    testBlockFormat.setAlignment(Qt::AlignHCenter);
    editCursor.insertBlock(testBlockFormat);

    QTextCharFormat defaultCharFormat;
    editCursor.insertText("a", defaultCharFormat);
    editCursor.insertText("b");
    editCursor.insertText("a\nb\nc");

    QTextTableFormat testTableFormat;
    testTableFormat.setWidth(200);
    testTableFormat.setCellSpacing(0);
    testTableFormat.setBorder(1);
    QTextTable *pTableTest = editCursor.insertTable(2, 3, testTableFormat);
    pTableTest->mergeCells(1, 1, 1, 2);
    //editCursor.insertText("(1,1)");
    //editCursor.movePosition(QTextCursor::NextBlock);
    //editCursor.insertText("(1,2)");
    //editCursor.movePosition(QTextCursor::NextBlock);
    //editCursor.insertText("(1,3)");
    //editCursor.movePosition(QTextCursor::NextBlock);
    //editCursor.insertText("(2,1)");
    //editCursor.movePosition(QTextCursor::NextBlock);
    //editCursor.insertText("(2,2)");
    //editCursor.movePosition(QTextCursor::NextBlock);
    //editCursor.insertText("(2,3)");
    //editCursor.movePosition(QTextCursor::NextBlock);
    //editCursor.insertText("(3,1)");

    int row = pTableTest->rows();
    int col = pTableTest->columns();
    QTextCursor tableCursorOld, tableCursorNew;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            tableCursorNew = pTableTest->cellAt(i, j).firstCursorPosition();
            if (tableCursorNew != tableCursorOld)
            {
                tableCursorOld = tableCursorNew;
                tableCursorNew.insertText(QString("(%1,%2)").arg(i).arg(j));
            }
        }
    }

    editCursor = ui->teReport->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setName(":/AppImages/Resources/images/open.png");
    editCursor.insertImage(imageFormat, QTextFrameFormat::InFlow);
    editCursor.insertImage(QImage(":/AppImages/Resources/images/save.png"));

    //QPrinter printer(QPrinter::ScreenResolution);
    QPrinter printer(QPrinter::HighResolution);
    QMarginsF pdfMarginsF(0.0, 0.0, 0.0, 0.0);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageMargins(pdfMarginsF, QPageLayout::Point);
    printer.setOutputFileName("E:\\Temp\\ReportComposeTest.pdf");
    pEditDoc->print(&printer);
}

void WinReportTesting::on_btnSavePDF_clicked()
{
    QString outXmlFilename = "E:/Temp/FopTest/MGI_ReportTestByQt.xml";
    XmlReportGenerator xmlReport;
    xmlReport.SetTagKeyword(XmlReportGenerator::TAG_BODY_PatientInfo_Name, QStringLiteral("名字："));
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_PatientInfo_Name, ui->leName->text());
    xmlReport.SetTagKeyword(XmlReportGenerator::TAG_BODY_PatientInfo_Age, QStringLiteral("岁数："));
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_PatientInfo_Age, ui->leAge->text());
    xmlReport.SetTagValue(XmlReportGenerator::TAG_BODY_UltrasoundImages_USImage, ui->leUsImgPath->text());
    xmlReport.SaveReportAsXml(outXmlFilename);

    QString serverIP = "localhost";
    int serverPort = 8000;

    mpSocketClient1->connectToHost(serverIP, serverPort);
    bool connectFlag1 = mpSocketClient1->waitForConnected(5000);

    int msgBodyLen = 0;
    int msgLen = 0;
    char *pTempMsgBuffer = nullptr;
    int count = 1;
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
            pMsgCommand->set_outfilename("E:/Temp/FopTest/QtReportTest.pdf");
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
            LogUtil::Debug(CODE_LOCATION, "write MsgID: %d", count);
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
    ui->lbImgPdfView->setPixmap(
        QPixmap::fromImage(
        image.scaled(ui->lbImgPdfView->width(), ui->lbImgPdfView->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation)
        )
    );

    ::fz_drop_pixmap(context, pixmap);
    ::fz_drop_document(context, document);
    ::fz_drop_context(context);
}

void WinReportTesting::on_btnPrintImgPDF_clicked()
{
    QPrinter imagePrinter(QPrinter::HighResolution);
    //imagePrinter.setPageSize(QPagedPaintDevice::A4);
    //imagePrinter.setOutputFileName("E:/Temp/FopTest/MGI_ReportTest_image_print.pdf");
    //imagePrinter.setResolution(QPrinter::ScreenResolution);
    imagePrinter.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
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
        pageBox = ::fz_make_rect(0, 0, 100, 100);
        ctm = ::fz_transform_page(pageBox, printerResolution, 0);
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

    QPainter imagePainter;
    bool resFlag = imagePainter.begin(&imagePrinter);
    QRect painterRect = imagePainter.viewport();
    imagePainter.drawImage(painterRect, image);
    resFlag = imagePainter.end();

    ::fz_drop_pixmap(context, pixmap);
    ::fz_drop_document(context, document);
    ::fz_drop_context(context);
}
