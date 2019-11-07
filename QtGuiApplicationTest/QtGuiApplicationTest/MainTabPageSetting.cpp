#include "MainTabPageSetting.h"

#include <QApplication>
#include <QScreen>
#include <QScrollBar>
#include <QDateTime>
#include <QTextBlock>
#include <QBuffer>
#include <QNetworkInterface>

#include <LogUtil.h>

#include "NotifyWidget.h"

class Base
{
public:
    Base()
    {
        int i = 0;
    }

    ~Base()
    {
        int i = 0;
    }

    void func()
    {
        int i = 0;
    }

    virtual void vfunc()
    {
        int i = 0;
    }
};

class Derived : public Base
{
public:
    Derived()
    {
        int i = 0;
    }

    ~Derived()
    {
        int i = 0;
    }

    void func()
    {
        int i = 0;
    }

    virtual void vfunc()
    {
        int i = 0;
    }
};


MainTabPageSetting::MainTabPageSetting(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.settingTxt->setText("setting ...");
    ui.tePdfTest->setText("abc");

    this->connect(ui.listWidget->verticalScrollBar(), &QScrollBar::valueChanged, this, &MainTabPageSetting::SlotListVScrollBarValueChanged);
    this->connect(ui.listWidget->verticalScrollBar(), &QScrollBar::rangeChanged, this, &MainTabPageSetting::SlotListVScrollBarRangeChanged);
}

MainTabPageSetting::~MainTabPageSetting()
{
}

void MainTabPageSetting::SlotListVScrollBarRangeChanged(int min, int max)
{
    LogUtil::Debug(CODE_LOCATION, "min:%d max:%d", min, max);
}

void MainTabPageSetting::SlotListVScrollBarValueChanged(int value)
{
    LogUtil::Debug(CODE_LOCATION, "value:%d", value);
}

void MainTabPageSetting::on_btnGeneratePDF_clicked()
{
    QTextDocument *pDocument = ui.tePdfTest->document();
    pDocument->setPlainText("Hello world");
}

void MainTabPageSetting::on_btnParseDocument_clicked()
{
    static int count = 0;
    NotifyWidget::ShowInformation(QString("Hello world!\n%1").arg(++count));
    return;

    QList<QScreen*> screenList = QApplication::screens();
    for each (QScreen *pScreen in screenList)
    {
        QRect availableGeometry = pScreen->availableGeometry();
        QSize availableSize = pScreen->availableSize();
        LogUtil::Debug(CODE_LOCATION, "AvailabeSize: %dx%d    AvaliableGeometry(x,y,width,height): %d,%d %dx%d",
                       availableSize.width(), availableSize.height(),
                       availableGeometry.x(), availableGeometry.y(), availableGeometry.width(), availableGeometry.height());

        QRect virtualGeometry = pScreen->availableVirtualGeometry();
        QSize virtualSize = pScreen->availableVirtualSize();
        LogUtil::Debug(CODE_LOCATION, "VirtualSize: %dx%d    VirtualGeometry(x,y,width,height): %d,%d %dx%d",
                       virtualSize.width(), virtualSize.height(),
                       virtualGeometry.x(), virtualGeometry.y(), virtualGeometry.width(), virtualGeometry.height());
    }
    return;

    //QHostAddress loopback("127.0.0.1");
    //if (loopback.isLoopback())
    //{
    //    int i = 0;
    //}

    //QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    //int nCnt = nets.count();
    //QString strMacAddr = "";
    //for (int i = 0; i < nCnt; i++)
    //{
    //    QString mac = nets[i].hardwareAddress();
    //    QString readableName = nets[i].humanReadableName();
    //    QString name = nets[i].name();
    //    QNetworkInterface::InterfaceFlags flags = nets[i].flags();
    //    LogUtil::Debug(CODE_LOCATION, "%s %s %s 0x%08X", mac.toUtf8().data(), readableName.toUtf8().data(), name.toUtf8().data(), flags);

    //    if (nets[i].flags().testFlag(QNetworkInterface::IsUp)
    //        && nets[i].flags().testFlag(QNetworkInterface::IsRunning)
    //        && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
    //    {
    //        strMacAddr = nets[i].hardwareAddress();
    //        break;
    //    }
    //}

    //int interfaceTest = 0;
    //return;

    //QString hospitalLogoStr = QCoreApplication::applicationDirPath() + "/Test1.jpg";
    //QPixmap hospitalLogo(hospitalLogoStr);
    //QByteArray logoPixBytes;
    //QBuffer logoBuffer(&logoPixBytes);
    //logoBuffer.open(QIODevice::WriteOnly);
    //hospitalLogo.save(&logoBuffer, "png");

    //QFile file(hospitalLogoStr);
    //if (file.open(QIODevice::ReadOnly))
    //{
    //    QByteArray fileBytes = file.readAll();
    //    QPixmap pixmap;
    //    pixmap.loadFromData(fileBytes);
    //    pixmap.save(QCoreApplication::applicationDirPath() + "/Test1_copy.jpg");
    //    file.close();
    //}


    //return;

    //int c[4] = { 1,2,3,4 };

    //Base *pBase = new Derived();
    //pBase->func();
    //pBase->vfunc();

    //int testSize = sizeof(Base);
    //Base *ptr = (Base*)malloc(sizeof(Base));

    //ptr->Base::Base();
    //ptr->func();
    //ptr->vfunc();

    //return;

    QTextDocument *pDocument = ui.tePdfTest->document();
    QTextFrame *pRootFrame = pDocument->rootFrame();
    int blockCount = pDocument->blockCount();
    QTextBlock tempBlock = pDocument->firstBlock();
    QTextBlock endBlock = pDocument->lastBlock();
    while (tempBlock.isValid())
    {
        QString text = tempBlock.text();
        tempBlock = tempBlock.next();
    }

    QTextCursor tempCursor = ui.tePdfTest->textCursor();
    tempCursor.insertText(QString("Insert Text Test %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz"))); // 2019-07-06T17:52:02.017578
    tempCursor.insertBlock();

    QString timeString("2019-07-06T17:52:02.987");
    QDateTime timeTest = QDateTime::fromString(timeString, "yyyy-MM-dd'T'hh:mm:ss.zzz");
    QString newTimeStr = timeTest.toString("yyyy-MM-ddThh:mm:ss.z");

    QString timeString2("2019-07-08 17:52:02.987");
    QDateTime timeTest2 = QDateTime::fromString(timeString2, "yyyy-MM-dd hh:mm:ss.zzz");
    QString newTimeStr2 = timeTest2.toString("yyyy-MM-dd hh:mm:ss.z");

    int i = 0;
}

void MainTabPageSetting::on_btnChangeItemCount_clicked()
{
    static int itemCount = 3;
    if (itemCount == 3)
    {
        itemCount = 10;
    }
    else
    {
        itemCount = 3;
    }

    QSize listWidgetSize = ui.listWidget->size();
    QSize listWidgetViewportSize = ui.listWidget->maximumViewportSize();
    QRect visualItemRect1 = ui.listWidget->visualItemRect(ui.listWidget->item(0));
    QRect visualItemRect2 = ui.listWidget->visualItemRect(ui.listWidget->item(1));

    QSize itemSizeHit1 = ui.listWidget->item(0)->sizeHint();
    QSize itemSizeHit2 = ui.listWidget->item(1)->sizeHint();

    ui.listWidget->clear();
    for (int i = 0; i < itemCount; ++i)
    {
        QWidget *pItemWidget = new QWidget(this);
        pItemWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        //if (i % 2 == 0)
        //{
        //    pItemWidget->setStyleSheet("background-color: red;");
        //}
        //else
        //{
        //    pItemWidget->setStyleSheet("background-color: blue;");
        //}
        //pItemWidget->setGeometry(0, 0, listWidgetViewportSize.width(), 30);
        //pItemWidget->resize(listWidgetViewportSize.width(), 30);
        //pItemWidget->setMinimumWidth(listWidgetViewportSize.width());
        //pItemWidget->resize(ui.listWidget->iconSize());
        //pItemWidget->setMinimumSize(ui.listWidget->iconSize());

        if (ui.listWidget->verticalScrollBar()->isVisible())
        {
            //pItemWidget->setMinimumWidth(listWidgetViewportSize.width() - ui.listWidget->verticalScrollBar()->width());
            pItemWidget->resize(200, 30);
        }
        else
        {
            pItemWidget->resize(100, 30);
            //pItemWidget->resize(listWidgetViewportSize.width(), 30);
            //pItemWidget->setMinimumWidth(listWidgetViewportSize.width());
            //pItemWidget->resize(ui.listWidget->iconSize());
            //pItemWidget->setMinimumSize(ui.listWidget->iconSize());
        }

        QLabel *pItemLabel = new QLabel(pItemWidget);
        pItemLabel->setText(QString("TestItem Label: %1 abcdefghijklmnopqrstuvwxyz").arg(i));
        QListWidgetItem *pListItem = new QListWidgetItem(QString("TestItem %1").arg(i), ui.listWidget);
        ui.listWidget->addItem(pListItem);
        ui.listWidget->setItemWidget(pListItem, pItemWidget);
    }
}

void MainTabPageSetting::on_btnShowVerticalScrollBar_clicked()
{
    static bool bShow = true;
    if (bShow)
    {
        bShow = false;
        ui.listWidget->verticalScrollBar()->setVisible(bShow);
    }
    else
    {
        bShow = true;
        ui.listWidget->verticalScrollBar()->setVisible(bShow);
    }
}
