#include "MainTabPageSetting.h"

#include <QScrollBar>
#include <QDateTime>
#include <QTextBlock>

#include <LogUtil.h>

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
    tempCursor.insertText("Insert Text Test");
    tempCursor.insertBlock();

    int i = 0;
}
