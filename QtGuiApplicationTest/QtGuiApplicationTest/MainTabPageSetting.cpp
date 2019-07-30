#include "MainTabPageSetting.h"

#include <QScrollBar>
#include <QDateTime>
#include <QTextBlock>

#include <LogUtil.h>

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
    int c[4] = { 1,2,3,4 };

    Base *pBase = new Derived();
    pBase->func();
    pBase->vfunc();

    int testSize = sizeof(Base);
    Base *ptr = (Base*)malloc(sizeof(Base));

    ptr->Base::Base();
    ptr->func();
    ptr->vfunc();

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

    int i = 0;
}
