#include "MiscellaneousRegularExpression.h"

#include <LogUtil.h>

MiscellaneousRegularExpression::MiscellaneousRegularExpression(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousRegularExpression::~MiscellaneousRegularExpression()
{
}

QString MiscellaneousRegularExpression::GetTitle()
{
    return QObject::tr("Regular Expression");
}

QString MiscellaneousRegularExpression::GetTitleTooltip()
{
    return QObject::tr("How to use QT regular exprssion.");
}

MiscellaneousTestGroup MiscellaneousRegularExpression::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousRegularExpression::GetItemID()
{
    return MiscellaneousTestItem::QT_Regular_Expression;
}

void MiscellaneousRegularExpression::LogFilenameMatchTest(const QString& targetStr)
{
    QRegExp regExpLogFilename("^((?!_)\\w)+_((?!_)\\w)+_\\d{8}_\\d{5}.log$");
    if (regExpLogFilename.exactMatch(targetStr))
    {
        LogUtil::Debug(CODE_LOCATION, "Regular Expression Test: %s match %s",
                       regExpLogFilename.pattern().toLocal8Bit().data(), targetStr.toLocal8Bit().data());
    }
    else
    {
        LogUtil::Debug(CODE_LOCATION, "Regular Expression Test: %s do not match %s",
                       regExpLogFilename.pattern().toLocal8Bit().data(), targetStr.toLocal8Bit().data());
    }
}

void MiscellaneousRegularExpression::LogFilenameCaptureTest(const QString& targetStr)
{
    LogUtil::Debug(CODE_LOCATION, "------------------------------------------------ Split Line Begin ------------------------------------------------");

    QRegExp regExpLogFilename("^((?!_)\\w+)_((?!_)\\w+)_(\\d{8})_(\\d{5}).log$");

    int pos = regExpLogFilename.indexIn(targetStr);
    if (pos > -1)
    {
        int captureCount = regExpLogFilename.captureCount();
        LogUtil::Debug(CODE_LOCATION, "Regular Expression Test: %s capture %d strings in %s",
                       regExpLogFilename.pattern().toLocal8Bit().data(), captureCount, targetStr.toLocal8Bit().data());

        QStringList capturedTexts = regExpLogFilename.capturedTexts();
        for each(QString capturedText in capturedTexts)
        {
            LogUtil::Debug(CODE_LOCATION, "Regular Expression Test captured: %s", capturedText.toLocal8Bit().data());
        }
    }
    else
    {
        LogUtil::Debug(CODE_LOCATION, "Regular Expression Test: %s do not match any string in %s",
                       regExpLogFilename.pattern().toLocal8Bit().data(), targetStr.toLocal8Bit().data());
    }

    LogUtil::Debug(CODE_LOCATION, "------------------------------------------------ Split Line End ------------------------------------------------");
}

void MiscellaneousRegularExpression::on_btnFullMatch_clicked()
{
    QString targetStr1 = "ProcessName_LoggerName_20200229_00001.log";
    QString targetStr2 = "ProcessName11_LoggerName_20200229_00001.log";
    QString targetStr3 = "ProcessName_Logger2Name_20200229_00001.log";
    QString targetStr4 = "ProcessName_LoggerName_120200229_00001.log";
    QString targetStr5 = "ProcessName_LoggerName_20200229_000012.log";

    this->LogFilenameMatchTest(targetStr1);
    this->LogFilenameMatchTest(targetStr2);
    this->LogFilenameMatchTest(targetStr3);
    this->LogFilenameMatchTest(targetStr4);
    this->LogFilenameMatchTest(targetStr5);

    QString tempStr = "sfn_aa";
    QRegExp regExpLogFilename("^((?!\_)\\w)+_((?!\_)\\w)+$");
    if (regExpLogFilename.exactMatch(tempStr))
    {
        LogUtil::Debug(CODE_LOCATION, "Regular Expression Test: %s match %s",
                       regExpLogFilename.pattern().toLocal8Bit().data(), tempStr.toLocal8Bit().data());
    }
    else
    {
        LogUtil::Debug(CODE_LOCATION, "Regular Expression Test: %s do not match %s",
                       regExpLogFilename.pattern().toLocal8Bit().data(), tempStr.toLocal8Bit().data());
    }
}

void MiscellaneousRegularExpression::on_btnCaptureGroup_clicked()
{
    QString targetStr1 = "ProcessName_LoggerName_20200229_00001.log";
    QString targetStr2 = "ProcessName11_LoggerName_20200229_00001.log";
    QString targetStr3 = "ProcessName_Logger2Name_20200229_00001.log";
    QString targetStr4 = "ProcessName_LoggerName_120200229_00001.log";
    QString targetStr5 = "ProcessName_LoggerName_20200229_000012.log";
    QString targetStr6 = QStringLiteral("ProcessName_���Ĳ���_20200229_00001.log");

    this->LogFilenameCaptureTest(targetStr1);
    this->LogFilenameCaptureTest(targetStr2);
    this->LogFilenameCaptureTest(targetStr3);
    this->LogFilenameCaptureTest(targetStr4);
    this->LogFilenameCaptureTest(targetStr5);
    this->LogFilenameCaptureTest(targetStr6);
}
