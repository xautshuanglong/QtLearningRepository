#include "MiscellaneousTesting.h"

#include "JCB_Logger/LogUtil.h"

#include "ui_MiscellaneousTesting.h"
#include "Core/FramelessWindowHelper.h"
#include "Miscellaneous/MiscellaneousBase.h"

// WinAPI Test
#include "Miscellaneous/MiscellaneousWinThread.h"
#include "Miscellaneous/MiscellaneousWinPrintDlg.h"
#include "Miscellaneous/MiscellaneousWinSocket.h"
#include "Miscellaneous/MiscellaneousWinEventTrace.h"
#include "Miscellaneous/MiscellaneousWinRegistry.h"
// Standard CPP
#include "Miscellaneous/MiscellaneousStdSharedPointer.h"
#include "Miscellaneous/MiscellaneousStdContainer.h"
#include "Miscellaneous/MiscellaneousStdConstructor.h"
#include "Miscellaneous/MiscellaneousStdCLanguageFeatures.h"
// Algorithm
#include "Miscellaneous/MiscellaneousAlgorithmLeetCode.h"
// QT Test
#include "Miscellaneous/MiscellaneousPrinterPDF.h"
#include "Miscellaneous/MiscellaneousSignalSlot.h"
#include "Miscellaneous/MiscellaneousPrinterWidget.h"
#include "Miscellaneous/MiscellaneousCustomTextEdit.h"
#include "Miscellaneous/MiscellaneousQVirtualKeyboard.h"
#include "Miscellaneous/MiscellaneousImageQImage.h"
#include "Miscellaneous/MiscellaneousImageQPixmap.h"
#include "Miscellaneous/MiscellaneousQObjectCast.h"
#include "Miscellaneous/MiscellaneousQTimerTest.h"
#include "Miscellaneous/MiscellaneousRegularExpression.h"
#include "Miscellaneous/MiscellaneousQMimeType.h"
#include "Miscellaneous/MiscellaneousExcelReadWrite.h"
#include "Miscellaneous/MiscellaneousQWebsocket.h"
#include "Miscellaneous/MiscellaneousQTcpSocket.h"
#include "Miscellaneous/MiscellaneousQNativeWindow.h"
// Database
#include "Miscellaneous/MiscellaneousWxSqlite3.h"
#include "Miscellaneous/MiscellaneousSqlCipher.h"
#include "Miscellaneous/MiscellaneousPostgresql.h"
// Utility Tools
#include "Miscellaneous/MiscellaneousStringUtil.h"
// Others
#include "Miscellaneous/MiscellaneousZip.h"
#include "Miscellaneous/MiscellaneousZLib.h"
#include "Miscellaneous/MiscellaneousDirectShow.h"
#include "Miscellaneous/MiscellaneousMemory.h"
#include "Miscellaneous/MiscellaneousFFmpeg.h"
#include "Miscellaneous/MiscellaneousTimeCode.h"

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
    ui->statusBar->setVisible(false);
    ui->twMiscellaneousTesting->clear();
    for (int i=0; i<static_cast<int>(MiscellaneousTestGroup::Max_Size); ++i)
    {
        QTreeWidgetItem *pTreeWidgetItem = new QTreeWidgetItem(ui->twMiscellaneousTesting, TYPE_GROUP);
        pTreeWidgetItem->setText(0, gMiscellaneousGroupInfo[i].groupTitle);
        mMapTestGroup[gMiscellaneousGroupInfo[i].groupID] = pTreeWidgetItem;
    }

    this->AppendTestPage(new MiscellaneousWinThread(this));
    this->AppendTestPage(new MiscellaneousWinPrintDlg(this));
    this->AppendTestPage(new MiscellaneousWinSocket(this));
    this->AppendTestPage(new MiscellaneousWinEventTrace(this));
    this->AppendTestPage(new MiscellaneousWinRegistry(this));
    this->AppendTestPage(new MiscellaneousStdSharedPointer(this));
    this->AppendTestPage(new MiscellaneousStdContainer(this));
    this->AppendTestPage(new MiscellaneousStdConstructor(this));
    this->AppendTestPage(new MiscellaneousStdCLanguageFeatures(this));
    this->AppendTestPage(new MiscellaneousAlgorithmLeetCode(this));
    this->AppendTestPage(new MiscellaneousSignalSlot(this));
    this->AppendTestPage(new MiscellaneousPrinterPDF(this));
    this->AppendTestPage(new MiscellaneousPrinterWidget(this));
    this->AppendTestPage(new MiscellaneousCustomTextEdit(this));
    this->AppendTestPage(new MiscellaneousQVirtualKeyboard(this));
    this->AppendTestPage(new MiscellaneousImageQImage(this));
    this->AppendTestPage(new MiscellaneousImageQPixmap(this));
    this->AppendTestPage(new MiscellaneousQObjectCast(this));
    this->AppendTestPage(new MiscellaneousQTimerTest(this));
    this->AppendTestPage(new MiscellaneousRegularExpression(this));
    this->AppendTestPage(new MiscellaneousQMimeType(this));
    this->AppendTestPage(new MiscellaneousExcelReadWrite(this));
    this->AppendTestPage(new MiscellaneousQWebSocket(this));
    this->AppendTestPage(new MiscellaneousQTcpSocket(this));
    this->AppendTestPage(new MiscellaneousQNativeWindow(this));
    this->AppendTestPage(new MiscellaneousWxSqlite3(this));
    this->AppendTestPage(new MiscellaneousSqlCipher(this));
    this->AppendTestPage(new MiscellaneousPostgresql(this));
    this->AppendTestPage(new MiscellaneousZip(this));
    this->AppendTestPage(new MiscellaneousZLib(this));
    this->AppendTestPage(new MiscellaneousDirectShow(this));
    this->AppendTestPage(new MiscellaneousMemory(this));
    this->AppendTestPage(new MiscellaneousFFmpeg(this));
    this->AppendTestPage(new MiscellaneousTimeCode(this));
    this->AppendTestPage(new MiscellaneousStringUtil(this));

    MiscellaneousBase *pCurMiscellaneousTest = static_cast<MiscellaneousBase*>(ui->swTestPageWidget->currentWidget());
    if (pCurMiscellaneousTest != Q_NULLPTR)
    {
        MiscellaneousTestGroup groupID = pCurMiscellaneousTest->GetGroupID();
        MiscellaneousTestItem itemID = pCurMiscellaneousTest->GetItemID();
        if (mMapTestGroup.contains(groupID))
        {
            QTreeWidgetItem* pChild = Q_NULLPTR;
            QTreeWidgetItem* parrent = mMapTestGroup[groupID];
            parrent->setExpanded(true);
            int childCount = parrent->childCount();
            for (int i = 0; i < childCount; ++i)
            {
                pChild = parrent->child(i);
                if (itemID == pChild->data(0, USER_DATA_ITEM_ID).value<MiscellaneousTestItem>())
                {
                    pChild->setSelected(true);
                    break;
                }
            }
        }
    }
}

void MiscellaneousTesting::AppendTestPage(MiscellaneousBase* pWidgetPage)
{
    int realPageIndex = ui->swTestPageWidget->addWidget(pWidgetPage);
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

void MiscellaneousTesting::ShowTestPage(QTreeWidgetItem *pItem)
{
    if (pItem != Q_NULLPTR)
    {
        MiscellaneousTestItem itemID = pItem->data(0, USER_DATA_ITEM_ID).value<MiscellaneousTestItem>();
        if (mMapTestPageIndex.contains(itemID))
        {
            ui->swTestPageWidget->setCurrentIndex(mMapTestPageIndex[itemID]);
        }
    }
}

void MiscellaneousTesting::closeEvent(QCloseEvent *event)
{
    emit SignalClosed();
}

void MiscellaneousTesting::on_twMiscellaneousTesting_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->type() == TYPE_ITEM)
    {
        this->ShowTestPage(item);
    }
    else if (item->type() == TYPE_GROUP)
    {
        QList<QTreeWidgetItem*> testGroupList = mMapTestGroup.values();
        for (int i = 0; i < testGroupList.size(); ++i)
        {
            QTreeWidgetItem *pItem = testGroupList[i];
            if (pItem == item)
            {
                if (pItem->isExpanded())
                {
                    pItem->setExpanded(false);
                }
                else
                {
                    pItem->setExpanded(true);
                    QTreeWidgetItem *pShowItem = pItem->child(0);
                    if (pShowItem != Q_NULLPTR)
                    {
                        pShowItem->setSelected(true);
                        this->ShowTestPage(pShowItem);
                    }
                }
            }
            else
            {
                pItem->setExpanded(false);
            }
        }
    }
}
