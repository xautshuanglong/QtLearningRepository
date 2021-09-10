#include "MainTabPageSetting.h"

#include <QApplication>
#include <QScreen>
#include <QScrollBar>
#include <QDateTime>
#include <QTextBlock>
#include <QBuffer>
#include <QMenu>
#include <QNetworkInterface>
#include <QStyledItemDelegate>

#include "JCB_Logger/LogUtil.h"

#include "Core/JcbMessageBox.h"
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

    QStyledItemDelegate *pItemDelegate = new QStyledItemDelegate();
    ui.cbTest->setItemDelegate(pItemDelegate);
    // 组合下拉框阴影消除
    ui.cbTest->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    //QWidget *pShadow = qobject_cast<QWidget*>(ui.cbTest->view()->parent());
    //if (pShadow != Q_NULLPTR)
    //{
    //    pShadow->setWindowFlags(pShadow->windowFlags() | Qt::NoDropShadowWindowHint);
    //    //pShadow->setAttribute(Qt::WA_TranslucentBackground);
    //}
    // 组合下拉框圆角处理
    ui.cbTest->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    this->connect(ui.lwTest->verticalScrollBar(), &QScrollBar::valueChanged, this, &MainTabPageSetting::SlotListVScrollBarValueChanged);
    this->connect(ui.lwTest->verticalScrollBar(), &QScrollBar::rangeChanged, this, &MainTabPageSetting::SlotListVScrollBarRangeChanged);

    // 按钮下拉菜单测试
    QMenu *pButtonMenu = new QMenu(this);
    QAction *pActionAdd = new QAction(pButtonMenu);
    QAction *pActionDelete = new QAction(pButtonMenu);
    pActionAdd->setText(QObject::tr("DropMenu-Add"));
    pActionDelete->setText(QObject::tr("DropMenu-Delete"));
    pButtonMenu->addAction(pActionAdd);
    pButtonMenu->addAction(pActionDelete);
    ui.btnDropMenu->setMenu(pButtonMenu);
    ui.btnDropMenu->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(pActionAdd, &QAction::triggered, this, &MainTabPageSetting::SlotDropMenuActionTriggeredAdd);
    connect(pActionDelete, &QAction::triggered, this, &MainTabPageSetting::SlotDropMenuActionTriggeredDelete);

    // TextEdit 测试
    QTextDocument *pTextDoc = ui.tePdfTest->document();
    connect(pTextDoc, SIGNAL(cursorPositionChanged(const QTextCursor &)), this, SLOT(SlotTextDocumentCursorPositionChanged(const QTextCursor &)));

    // 动态属性控制样式
    confirm = new QPushButton(this);
    confirm->setText("confirm");
    confirm->setProperty("level", "dangrous");
    confirm->move(10, 10);

    cancel = new QPushButton(this);
    cancel->setText("cancel");
    cancel->setProperty("level", "normal");
    cancel->setProperty("urgent", true);
    cancel->move(150, 10);

    this->setStyleSheet("QPushButton { color: #FFF; border: 0; height: 30px; width: 100px; }"
        "QPushButton[level='dangrous'] { background-color: red; }"
        "QPushButton[level='normal'] { background-color: blue; }");
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

void MainTabPageSetting::SlotDropMenuActionTriggeredAdd()
{
    int i = 0;
}

void MainTabPageSetting::SlotDropMenuActionTriggeredDelete()
{
    int i = 0;
}

void MainTabPageSetting::on_btnGeneratePDF_clicked()
{
    QTextDocument *pDocument = ui.tePdfTest->document();
    pDocument->setPlainText("Hello world");
}

void MainTabPageSetting::on_btnParseDocument_clicked()
{
    //static int count = 0;
    //NotifyWidget::ShowInformation(QString("Hello world!\n%1").arg(++count));
    //return;

    //QList<QScreen*> screenList = QApplication::screens();
    //for each (QScreen *pScreen in screenList)
    //{
    //    QRect availableGeometry = pScreen->availableGeometry();
    //    QSize availableSize = pScreen->availableSize();
    //    LogUtil::Debug(CODE_LOCATION, "AvailabeSize: %dx%d    AvaliableGeometry(x,y,width,height): %d,%d %dx%d",
    //                   availableSize.width(), availableSize.height(),
    //                   availableGeometry.x(), availableGeometry.y(), availableGeometry.width(), availableGeometry.height());

    //    QRect virtualGeometry = pScreen->availableVirtualGeometry();
    //    QSize virtualSize = pScreen->availableVirtualSize();
    //    LogUtil::Debug(CODE_LOCATION, "VirtualSize: %dx%d    VirtualGeometry(x,y,width,height): %d,%d %dx%d",
    //                   virtualSize.width(), virtualSize.height(),
    //                   virtualGeometry.x(), virtualGeometry.y(), virtualGeometry.width(), virtualGeometry.height());
    //}
    //return;

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

    QFont linkFont("Times", 25);
    linkFont.setUnderline(true);
    QTextCharFormat charFormatOld = tempCursor.blockCharFormat();
    QTextCharFormat charFormatNew = charFormatOld;
    charFormatNew.setFont(linkFont);
    charFormatNew.setForeground(QBrush(Qt::blue));
    tempCursor.setCharFormat(charFormatNew);
    tempCursor.insertText("CharFormatTesting");
    tempCursor.setCharFormat(charFormatOld);
    tempCursor.insertText("Recover to old format");

    QTextFrameFormat frameFormat;
    QTextFrame *pCurFrame = tempCursor.insertFrame(frameFormat);
    QTextCursor curFrameCursor = pCurFrame->firstCursorPosition();
    curFrameCursor.insertText("Text inside new frame");
    curFrameCursor.insertText("append text");

    tempCursor.insertText("Old Cursor Text");
    //QString timeString("2019-07-06T17:52:02.987");
    //QDateTime timeTest = QDateTime::fromString(timeString, "yyyy-MM-dd'T'hh:mm:ss.zzz");
    //QString newTimeStr = timeTest.toString("yyyy-MM-ddThh:mm:ss.z");

    //QString timeString2("2019-07-08 17:52:02.987");
    //QDateTime timeTest2 = QDateTime::fromString(timeString2, "yyyy-MM-dd hh:mm:ss.zzz");
    //QString newTimeStr2 = timeTest2.toString("yyyy-MM-dd hh:mm:ss.z");

    int i = 0;
}

void MainTabPageSetting::on_btnMessageBox_clicked()
{
    QString lightGreenFlag = ui.cbTest->property("lightGreenFlag").toString();
    if (lightGreenFlag == "true")
    {
        ui.cbTest->setProperty("lightGreenFlag", "false");
        confirm->setProperty("level", "dangrous");
        cancel->setProperty("level", "normal");
    } 
    else
    {
        ui.cbTest->setProperty("lightGreenFlag", "true");
        confirm->setProperty("level", "normal");
        cancel->setProperty("level", "dangrous");
    }

    style()->unpolish(ui.cbTest);
    style()->polish(ui.cbTest);

    style()->unpolish(confirm);
    style()->polish(confirm);

    style()->unpolish(cancel);
    style()->polish(cancel);

    QMessageBox msgBox(this);
    msgBox.setText("Testing content ... Testing content ... Testing content ... Testing content ... Testing content ... Testing content ...");
    msgBox.setWindowTitle("Window Title");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.exec();

    JcbMessageBox jcbMsgBox(this);
    jcbMsgBox.setText("Testing content ... Testing content ... Testing content ...\nTesting content ... Testing content ... Testing content ...");
    jcbMsgBox.setWindowTitle("Window Title");
    jcbMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    jcbMsgBox.exec();
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

    QSize listWidgetSize = ui.lwTest->size();
    QSize listWidgetViewportSize = ui.lwTest->maximumViewportSize();
    QRect visualItemRect1 = ui.lwTest->visualItemRect(ui.lwTest->item(0));
    QRect visualItemRect2 = ui.lwTest->visualItemRect(ui.lwTest->item(1));

    QSize itemSizeHit1 = ui.lwTest->item(0)->sizeHint();
    QSize itemSizeHit2 = ui.lwTest->item(1)->sizeHint();

    ui.lwTest->clear();
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
        //pItemWidget->resize(ui.lwTest->iconSize());
        //pItemWidget->setMinimumSize(ui.lwTest->iconSize());

        if (ui.lwTest->verticalScrollBar()->isVisible())
        {
            //pItemWidget->setMinimumWidth(listWidgetViewportSize.width() - ui.lwTest->verticalScrollBar()->width());
            pItemWidget->resize(200, 30);
        }
        else
        {
            pItemWidget->resize(100, 30);
            //pItemWidget->resize(listWidgetViewportSize.width(), 30);
            //pItemWidget->setMinimumWidth(listWidgetViewportSize.width());
            //pItemWidget->resize(ui.lwTest->iconSize());
            //pItemWidget->setMinimumSize(ui.lwTest->iconSize());
        }

        QLabel *pItemLabel = new QLabel(pItemWidget);
        pItemLabel->setText(QString("TestItem Label: %1 abcdefghijklmnopqrstuvwxyz").arg(i));
        QListWidgetItem *pListItem = new QListWidgetItem(QString("TestItem %1").arg(i), ui.lwTest);
        ui.lwTest->addItem(pListItem);
        ui.lwTest->setItemWidget(pListItem, pItemWidget);
    }
}

void MainTabPageSetting::on_btnShowVerticalScrollBar_clicked()
{
    static bool bShow = true;
    if (bShow)
    {
        bShow = false;
        ui.lwTest->verticalScrollBar()->setVisible(bShow);
    }
    else
    {
        bShow = true;
        ui.lwTest->verticalScrollBar()->setVisible(bShow);
    }
}

void MainTabPageSetting::on_btnDropMenu_clicked()
{
    int i = 0;
}

void MainTabPageSetting::on_btnDropMenu_customContextMenuRequested(const QPoint &pos)
{
    int i = 0;
}

void MainTabPageSetting::on_tePdfTest_cursorPositionChanged()
{
    QTextCursor cursor = ui.tePdfTest->textCursor();
    QTextBlock curBlock = cursor.block();
    QString text = curBlock.text();
    LogUtil::Info(CODE_LOCATION, "Current block's text: %s", text.toUtf8().data());
}

void MainTabPageSetting::SlotTextDocumentCursorPositionChanged(const QTextCursor &cursor)
{
    QTextBlock curBlock = cursor.block();
    QString text = curBlock.text();
    int i = 0;
}
