#include "WinReportTesting.h"
#include "ui_WinReportTesting.h"

#include <QFile>
#include <QTextDocument>

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

void WinReportTesting::on_btnPrintTest_clicked()
{
    this->QTextDocumentTest();
}
