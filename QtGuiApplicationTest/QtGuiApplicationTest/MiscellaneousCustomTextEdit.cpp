#include "MiscellaneousCustomTextEdit.h"

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

void MiscellaneousCustomTextEdit::on_btnAppendText_clicked()
{
    QString appendText = ui.leAppendText->text();
    appendText.replace("\\n", "\n");
    appendText.replace("\\t", "\t");
    if (!appendText.isEmpty())
    {
        ui.teOriginal->append(appendText);
        ui.teCustom->append(appendText);
    }
}
