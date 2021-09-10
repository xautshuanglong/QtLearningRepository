#include "MiscellaneousCustomTextEdit.h"

#include <windows.h>

#include <QTextDocument>
#include <QTextBlock>
#include <QDateTime>

#include "JCB_Logger/LogUtil.h"

MiscellaneousCustomTextEdit::MiscellaneousCustomTextEdit(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousCustomTextEdit::~MiscellaneousCustomTextEdit()
{
}

QString MiscellaneousCustomTextEdit::GetTitle()
{
    return QObject::tr("Custom TextEdit");
}

QString MiscellaneousCustomTextEdit::GetTitleTooltip()
{
    return QObject::tr("Custom text edit for right click menu.");
}

MiscellaneousTestGroup MiscellaneousCustomTextEdit::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousCustomTextEdit::GetItemID()
{
    return MiscellaneousTestItem::QT_Custom_TextEdit;
}
 
void MiscellaneousCustomTextEdit::paintEvent(QPaintEvent* pEvent)
{
    QWidget::paintEvent(pEvent);
}

void MiscellaneousCustomTextEdit::AppendRichText(QTextEdit* pTextEdit)
{
    QTextDocument* pDocument = pTextEdit->document();
    QTextFrame* pRootFrame = pDocument->rootFrame();
    int blockCount = pDocument->blockCount();
    QTextBlock tempBlock = pDocument->firstBlock();
    QTextBlock endBlock = pDocument->lastBlock();
    while (tempBlock.isValid())
    {
        QString text = tempBlock.text();
        tempBlock = tempBlock.next();
    }

    QTextCursor tempCursor = pTextEdit->textCursor();
    tempCursor.insertText(QString("Insert Text Test %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz"))); // 2019-07-06T17:52:02.017578
    tempCursor.insertText(QStringLiteral("÷4+×()×÷"));
    tempCursor.insertBlock();

    QFont linkFont("Times", 25);
    linkFont.setUnderline(true);
    QTextCharFormat charFormatOld = tempCursor.blockCharFormat();
    QTextCharFormat charFormatNew = charFormatOld;
    charFormatNew.setFont(linkFont);
    charFormatNew.setForeground(QBrush(Qt::blue));
    tempCursor.setCharFormat(charFormatNew);
    tempCursor.insertText("CharFormatTesting,");
    tempCursor.insertText("AnotherInsert");
    tempCursor.insertText(" white space split");
    tempCursor.insertBlock();
    tempCursor.insertText("The text inside new block");
    tempCursor.setCharFormat(charFormatOld);
    tempCursor.insertBlock();
    tempCursor.insertText("Recover to old format");

    QTextFrameFormat frameFormat;
    QTextFrame* pCurFrame = tempCursor.insertFrame(frameFormat);
    QTextCursor curFrameCursor = pCurFrame->firstCursorPosition();
    curFrameCursor.insertText("Text inside new frame");
    curFrameCursor.insertText("append text");

    tempCursor.insertText("Old Cursor Text");

    // 插入列表测试
    QTextBlockFormat oldFormat = tempCursor.blockFormat();
    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDecimal);
    listFormat.setNumberPrefix("[");
    listFormat.setNumberSuffix("]");
    tempCursor.insertList(listFormat);
    tempCursor.insertText("List Item 1");
    tempCursor.insertBlock();
    tempCursor.insertText("List Item 2");
    tempCursor.insertBlock();
    tempCursor.insertText("List Item 3");
    tempCursor.insertBlock();
    tempCursor.setBlockFormat(oldFormat);
    tempCursor.insertText("out of the list");
}

void MiscellaneousCustomTextEdit::AppendCalendarTxet(QTextEdit* pTextEdit)
{
    QTextCursor cursor(pTextEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format(cursor.charFormat());
    format.setFontFamily("Courier");
    QTextCharFormat boldFormat = format;
    boldFormat.setFontWeight(QFont::Bold);
    cursor.insertBlock();
    cursor.insertText(" ", boldFormat);
    QDate date = QDate::currentDate();
    int year = date.year(), month = date.month();
    for (int weekDay = 1; weekDay <= 7; ++weekDay)
    {
        cursor.insertText(QString("%1 ").arg(QLocale::system().dayName(weekDay), 3), boldFormat);
    }
    cursor.insertBlock();
    cursor.insertText(" ", format);
    for (int column = 1; column < QDate(year, month, 1).dayOfWeek(); ++column)
    {
        cursor.insertText("    ", format);
    }
    for (int day = 1; day <= date.daysInMonth(); ++day)
    {
        int weekDay = QDate(year, month, day).dayOfWeek();
        if (QDate(year, month, day) == date)
            cursor.insertText(QString("%1 ").arg(day, 3), boldFormat);
        else
            cursor.insertText(QString("%1 ").arg(day, 3), format);
        if (weekDay == 7)
        {
            cursor.insertBlock();
            cursor.insertText(" ", format);
        }
    }
}

void MiscellaneousCustomTextEdit::on_teCustom_cursorPositionChanged()
{
     QTextBlock cursorBlock = ui.teCustom->textCursor().block();
     LogUtil::Debug(CODE_LOCATION, "Cursor text : %s", cursorBlock.text().toUtf8().data());
     if (cursorBlock.charFormat().font().underline())
     {
         LogUtil::Debug(CODE_LOCATION, "Underline text : %s", cursorBlock.text().toUtf8().data());
     }
}

void MiscellaneousCustomTextEdit::on_teCustom_currentCharFormatChanged(const QTextCharFormat& format)
{
    if (format.font().underline())
    {
        QString cursorText = ui.teCustom->textCursor().block().text();
        LogUtil::Debug(CODE_LOCATION, "FormateChanged : %s", cursorText.toUtf8().data());
    }
}

void MiscellaneousCustomTextEdit::on_btnAppendText_clicked()
{
    //DWORD fileAttributes = ::GetFileAttributesA("E:/Temp/CreateFileA_Test/");
    ////HANDLE hDevice = ::CreateFileA("E:/test.json", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    //HANDLE hDevice = ::CreateFileA("E:\\Temp\\CreateFileA_Test", GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ////HANDLE hDevice = ::CreateFileA("\\\\.\\PHYSICALDRIVE0", GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //if (hDevice == INVALID_HANDLE_VALUE)
    //{
    //    DWORD resCode = ::GetLastError();
    //    int i = 0;
    //}
    //else
    //{
    //    ::CloseHandle(hDevice);
    //}
    //return;

    HWND pHwnd = NULL;
    HWND ppHwnd = NULL;
    HWND pppHwnd = NULL;
    HWND ppppHwnd = NULL;
    HWND pppppHwnd = NULL;
    HWND hwnd = (HWND)this->winId();
    QString pObjName;
    QString ppObjName;
    QString pppObjName;
    QString ppppObjName;
    QString pppppObjName;
    QString objName = this->objectName();
    if (this->parentWidget())
    {
        pHwnd = (HWND)this->parentWidget()->winId();
        pObjName = this->parentWidget()->objectName();

        if (this->parentWidget()->parentWidget())
        {
            ppHwnd = (HWND)this->parentWidget()->parentWidget()->winId();
            ppObjName = this->parentWidget()->parentWidget()->objectName();

            if (this->parentWidget()->parentWidget()->parentWidget())
            {
                pppHwnd = (HWND)this->parentWidget()->parentWidget()->parentWidget()->winId();
                pppObjName= this->parentWidget()->parentWidget()->parentWidget()->objectName();

                if (this->parentWidget()->parentWidget()->parentWidget()->parentWidget())
                {
                    ppppHwnd = (HWND)this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->winId();
                    pppppObjName = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->objectName();

                    if (this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget())
                    {
                        pppppHwnd = (HWND)this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget()->winId();
                        pppppObjName = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget()->objectName();
                    }
                }
            }
        }
    }
    QWindow *pThisWindow = this->windowHandle();

    QString appendText = ui.leAppendText->text();
    appendText.replace("\\n", "\n");
    appendText.replace("\\t", "\t");
    if (!appendText.isEmpty())
    {
        ui.teOriginal->append(appendText);
        ui.teCustom->append(appendText);
    }
}

void MiscellaneousCustomTextEdit::on_btnTestEntry_clicked()
{
    this->AppendRichText(ui.teCustom);
    this->AppendRichText(ui.teOriginal);

    // 日历参考
    for (int i = 0; i < 10; ++i)
    {
        this->AppendCalendarTxet(ui.teCustom);
        this->AppendCalendarTxet(ui.teOriginal);
    }

    // 文档元素遍历
    int blockNum = 1;
    QTextDocument* pDocument = ui.teCustom->document();
    int blockCount = pDocument->blockCount();
    QTextBlock pBlock = pDocument->firstBlock();
    while (pBlock.isValid())
    {
        int lineCount = pBlock.lineCount();
        int charLength = pBlock.length();

        LogUtil::Debug(CODE_LOCATION, "Block[%d]   lines:%d   length:%d   blockNum:%d  %s", blockNum, lineCount, charLength, pBlock.blockNumber(), pBlock.text().toUtf8().data());
        pBlock = pBlock.next();
        ++blockNum;
    }
}
