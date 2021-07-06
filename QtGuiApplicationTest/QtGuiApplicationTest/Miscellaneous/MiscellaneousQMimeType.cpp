#include "MiscellaneousQMimeType.h"

#include <QFileDialog>
#include <QMimeDatabase>

MiscellaneousQMimeType::MiscellaneousQMimeType(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousQMimeType::~MiscellaneousQMimeType()
{
}

QString MiscellaneousQMimeType::GetTitle()
{
    return QObject::tr("QMimeType");
}

QString MiscellaneousQMimeType::GetTitleTooltip()
{
    return QObject::tr("Check file's mime type");
}

MiscellaneousTestGroup MiscellaneousQMimeType::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQMimeType::GetItemID()
{
    return MiscellaneousTestItem::QT_Mime_Type;
}

void MiscellaneousQMimeType::on_btnFileOpen_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select file to check", ".", "Any (*.*)");
    ui.leFilePath->setText(filename);

    QMimeDatabase mimeDB;
    QMimeType fileType = mimeDB.mimeTypeForFile(filename);
    ui.teTextOutput->appendPlainText(fileType.name());
}
