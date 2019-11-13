#include "MiscellaneousTesting.h"

#include <QPrinter>
#include <QPrinterInfo>
#include <QPainter>

#include <windows.h>
#include <process.h>

#include <LogUtil.h>

#include "ui_MiscellaneousTesting.h"
#include "FramelessWindowHelper.h"
#include "MiscellaneousBeginThreadEx.h"
#include "MiscellaneousPrinterPDF.h"

enum TreeItemType
{
    TYPE_GROUP,  // 组，可展开
    TYPE_ITEM    // 项，不可展开
};

enum ItemDataRole
{
    USER_DATA_GROPU_ID = Qt::UserRole + 1,
    USER_DATA_ITEM_ID  = Qt::UserRole + 2,
};

MiscellaneousTesting::MiscellaneousTesting(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , mpFramelessHelper(Q_NULLPTR)
    , ui(new Ui::MiscellaneousTesting())
    , m_beginThreadHandle(0)
    , m_threadStopFlag(false)
{
    ui->setupUi(this);
    mpFramelessHelper = new FramelessWindowHelper(this);

    this->InitializeUI();
}

MiscellaneousTesting::~MiscellaneousTesting()
{
    if (mpFramelessHelper != Q_NULLPTR)
    {
        delete mpFramelessHelper;
    }
}

void MiscellaneousTesting::InitializeUI()
{
    ui->twMiscellaneousTesting->clear();
    for (int i=0; i<static_cast<int>(MiscellaneousTestGroup::Max_Size); ++i)
    {
        QTreeWidgetItem *pTreeWidgetItem = new QTreeWidgetItem(ui->twMiscellaneousTesting, TYPE_GROUP);
        pTreeWidgetItem->setText(0, gMiscellaneousGroupInfo[i].groupTitle);
        mMapTestGroup[gMiscellaneousGroupInfo[i].groupID] = pTreeWidgetItem;
    }

    this->AppendTestPage(new MiscellaneousBeginThreadEx(this));
    this->AppendTestPage(new MiscellaneousPrinterPDF(this));
}

void MiscellaneousTesting::AppendTestPage(MiscellaneousBase* pWidgetPage)
{
    int pageIndex = static_cast<int>(pWidgetPage->GetItemID());
    int realPageIndex = ui->swTestPageWidget->insertWidget(pageIndex, pWidgetPage);
    MiscellaneousTestItem itemID = pWidgetPage->GetItemID();
    mMapTestPageIndex[itemID] = realPageIndex;
    MiscellaneousTestGroup groupID = pWidgetPage->GetGroupID();
    if (mMapTestGroup.contains(groupID))
    {
        QTreeWidgetItem *pTreeWidgetItem = new QTreeWidgetItem(mMapTestGroup[groupID], TYPE_ITEM);
        pTreeWidgetItem->setData(0, USER_DATA_GROPU_ID, QVariant::fromValue(groupID));
        pTreeWidgetItem->setData(0, USER_DATA_ITEM_ID, QVariant::fromValue(itemID));
        pTreeWidgetItem->setText(0, pWidgetPage->GetTitle());
        pTreeWidgetItem->setToolTip(0, pWidgetPage->GetTitleTooltip());
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
    if (item->type() == TYPE_ITEM)
    {
        MiscellaneousTestItem itemID = item->data(0, USER_DATA_ITEM_ID).value<MiscellaneousTestItem>();
        if (mMapTestPageIndex.contains(itemID))
        {
            ui->swTestPageWidget->setCurrentIndex(mMapTestPageIndex[itemID]);
        }
    }
}

void MiscellaneousTesting::Run()
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
    pMiscellaneous->Run();
    return 0;
}
