#include "MiscellaneousTesting.h"

#include <LogUtil.h>

#include "ui_MiscellaneousTesting.h"
#include "FramelessWindowHelper.h"
#include "MiscellaneousBase.h"

// WinAPI Test
#include "MiscellaneousBeginThreadEx.h"
// QT Test
#include "MiscellaneousPrinterPDF.h"
#include "MiscellaneousPrinterWidget.h"
#include "MiscellaneousImageQImage.h"
#include "MiscellaneousImageQPixmap.h"
// Database
#include "MiscellaneousWxSqlite3.h"

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
    this->AppendTestPage(new MiscellaneousPrinterPDF(this));
    this->AppendTestPage(new MiscellaneousPrinterWidget(this));
    this->AppendTestPage(new MiscellaneousImageQImage(this));
    this->AppendTestPage(new MiscellaneousImageQPixmap(this));
    this->AppendTestPage(new MiscellaneousWxSqlite3(this));

    MiscellaneousBase *pCurMiscellaneousTest = static_cast<MiscellaneousBase*>(ui->swTestPageWidget->currentWidget());
    MiscellaneousTestGroup groupID = pCurMiscellaneousTest->GetGroupID();
    if (mMapTestGroup.contains(groupID))
    {
        mMapTestGroup[groupID]->setExpanded(true);
    }
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
    else if (item->type() == TYPE_GROUP)
    {
        QList<QTreeWidgetItem*> testGroupList = mMapTestGroup.values();
        for each (QTreeWidgetItem* pItem in testGroupList)
        {
            if (pItem == item)
            {
                pItem->setExpanded(!pItem->isExpanded());
            }
            else
            {
                pItem->setExpanded(false);
            }
        }
    }
}
