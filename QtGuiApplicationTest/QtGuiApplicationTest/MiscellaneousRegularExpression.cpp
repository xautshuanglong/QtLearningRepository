#include "MiscellaneousRegularExpression.h"

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

void MiscellaneousRegularExpression::on_btnFullMatch_clicked()
{
    int i = 0;
}

void MiscellaneousRegularExpression::on_btnCaptureGroup_clicked()
{
    int i = 0;
}
