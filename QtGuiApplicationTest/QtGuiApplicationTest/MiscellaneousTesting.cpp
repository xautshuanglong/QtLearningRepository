#include "MiscellaneousTesting.h"

#include <QPrinter>
#include <QPrinterInfo>
#include <QPainter>

#include <windows.h>
#include <process.h>

#include <LogUtil.h>

#include "ui_MiscellaneousTesting.h"
#include "FramelessWindowHelper.h"

MiscellaneousTesting::MiscellaneousTesting(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , mpFramelessHelper(Q_NULLPTR)
    , ui(new Ui::MiscellaneousTesting())
    , m_beginThreadHandle(0)
    , m_threadStopFlag(false)
{
    ui->setupUi(this);
    mpFramelessHelper = new FramelessWindowHelper(this);
}

MiscellaneousTesting::~MiscellaneousTesting()
{
    if (mpFramelessHelper != Q_NULLPTR)
    {
        delete mpFramelessHelper;
    }
}

void MiscellaneousTesting::closeEvent(QCloseEvent *event)
{
    emit SignalClosed();
}

void MiscellaneousTesting::on_btnWinApiThreadStart_clicked()
{
    m_threadStopFlag = false;
    m_beginThreadHandle = _beginthreadex(NULL, 0, MiscellaneousTesting::ThreadProc, (void*)this, 0, NULL);
}

void MiscellaneousTesting::on_btnWinApiThreadStop_clicked()
{
    m_threadStopFlag = true;
    ::WaitForSingleObject((HANDLE)m_beginThreadHandle, INFINITE);
    int i = 0;
}

void MiscellaneousTesting::on_btnQPrinterTest_clicked()
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

void MiscellaneousTesting::on_twMiscellaneousTesting_itemClicked(QTreeWidgetItem *item, int column)
{
    int i = 0;
}

void MiscellaneousTesting::run()
{
    int count = 0;
    while (count < 1000)
    {
        ::_sleep(10);
        ++count;
        if (m_threadStopFlag)
        {
            break;
        }
    }
    int i = 0;
}

unsigned int _stdcall MiscellaneousTesting::ThreadProc(void *pArg)
{
    MiscellaneousTesting *pMiscellaneous = (MiscellaneousTesting*)pArg;
    pMiscellaneous->run();
    return 0;
}
