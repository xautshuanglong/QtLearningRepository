#include "MiscellaneousTesting.h"

#include <LogUtil.h>

#include "ui_MiscellaneousTesting.h"
#include "FramelessWindowHelper.h"
#include "MiscellaneousBase.h"

// WinAPI Test
#include "MiscellaneousBeginThreadEx.h"
// Standard CPP
#include "MiscellaneousStdSharedPointer.h"
#include "MiscellaneousStdConstructor.h"
#include "MiscellaneousCLanguageFeatures.h"
// Algorithm
#include "MiscellaneousAlgorithmLeetCode.h"
// QT Test
#include "MiscellaneousPrinterPDF.h"
#include "MiscellaneousSignalSlot.h"
#include "MiscellaneousPrinterWidget.h"
#include "MiscellaneousImageQImage.h"
#include "MiscellaneousImageQPixmap.h"
#include "MiscellaneousQObjectCast.h"
#include "MiscellaneousRegularExpression.h"
#include "MiscellaneousExcelReadWrite.h"
#include "MiscellaneousQWebsocket.h"
#include "MiscellaneousQTcpSocket.h"
// Database
#include "MiscellaneousWxSqlite3.h"
#include "MiscellaneousSqlCipher.h"
#include "MiscellaneousPostgresql.h"
// Utility Tools
#include "MiscellaneousStringUtil.h"
// Others
#include "MiscellaneousZip.h"
#include "MiscellaneousZLib.h"

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
    ui->twMiscellaneousTesting->clear();
    for (int i=0; i<static_cast<int>(MiscellaneousTestGroup::Max_Size); ++i)
    {
        QTreeWidgetItem *pTreeWidgetItem = new QTreeWidgetItem(ui->twMiscellaneousTesting, TYPE_GROUP);
        pTreeWidgetItem->setText(0, gMiscellaneousGroupInfo[i].groupTitle);
        mMapTestGroup[gMiscellaneousGroupInfo[i].groupID] = pTreeWidgetItem;
    }

    this->AppendTestPage(new MiscellaneousBeginThreadEx(this));
    this->AppendTestPage(new MiscellaneousStdSharedPointer(this));
    this->AppendTestPage(new MiscellaneousStdConstructor(this));
    this->AppendTestPage(new MiscellaneousCLanguageFeatures(this));
    this->AppendTestPage(new MiscellaneousAlgorithmLeetCode(this));
    this->AppendTestPage(new MiscellaneousSignalSlot(this));
    this->AppendTestPage(new MiscellaneousPrinterPDF(this));
    this->AppendTestPage(new MiscellaneousPrinterWidget(this));
    this->AppendTestPage(new MiscellaneousImageQImage(this));
    this->AppendTestPage(new MiscellaneousImageQPixmap(this));
    this->AppendTestPage(new MiscellaneousQObjectCast(this));
    this->AppendTestPage(new MiscellaneousRegularExpression(this));
    this->AppendTestPage(new MiscellaneousExcelReadWrite(this));
    this->AppendTestPage(new MiscellaneousQWebSocket(this));
    this->AppendTestPage(new MiscellaneousQTcpSocket(this));
    this->AppendTestPage(new MiscellaneousWxSqlite3(this));
    this->AppendTestPage(new MiscellaneousSqlCipher(this));
    this->AppendTestPage(new MiscellaneousPostgresql(this));
    this->AppendTestPage(new MiscellaneousZip(this));
    this->AppendTestPage(new MiscellaneousZLib(this));
    this->AppendTestPage(new MiscellaneousStringUtil(this));

    MiscellaneousBase *pCurMiscellaneousTest = static_cast<MiscellaneousBase*>(ui->swTestPageWidget->currentWidget());
    MiscellaneousTestGroup groupID = pCurMiscellaneousTest->GetGroupID();
    MiscellaneousTestItem itemID = pCurMiscellaneousTest->GetItemID();
    if (mMapTestGroup.contains(groupID))
    {
        QTreeWidgetItem *pChild = Q_NULLPTR;
        QTreeWidgetItem *parrent = mMapTestGroup[groupID];
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
        for each (QTreeWidgetItem* pItem in testGroupList)
        {
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
