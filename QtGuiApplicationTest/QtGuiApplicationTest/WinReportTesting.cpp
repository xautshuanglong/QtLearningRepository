#include "WinReportTesting.h"
#include "ui_WinReportTesting.h"

#include <QFile>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextTable>
#include <QPrinter>

#include <MemUtil.h>
#include "FramelessWindowHelper.h"

using namespace SL::Util;

WinReportTesting::WinReportTesting(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , ui(new Ui::WinReportTesting())
    , mpFramelessHelper(Q_NULLPTR)
{
    ui->setupUi(this);
    mpFramelessHelper = new FramelessWindowHelper(this);

    ui->teReport->setReadOnly(true);
}

WinReportTesting::~WinReportTesting()
{
    MemUtil::RelesePointer(&mpFramelessHelper);
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
    this->QTextEditDocumentTest();
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
