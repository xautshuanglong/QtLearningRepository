#include "MiscellaneousQVirtualKeyboard.h"

#include <windows.h>

#include <QTextDocument>
#include <QTextBlock>
#include <QDateTime>

#include <LogUtil.h>

MiscellaneousQVirtualKeyboard::MiscellaneousQVirtualKeyboard(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousQVirtualKeyboard::~MiscellaneousQVirtualKeyboard()
{
}

QString MiscellaneousQVirtualKeyboard::GetTitle()
{
    return QObject::tr("Virtual Keyboard");
}

QString MiscellaneousQVirtualKeyboard::GetTitleTooltip()
{
    return QObject::tr("Custom virtual keyboard.");
}

MiscellaneousTestGroup MiscellaneousQVirtualKeyboard::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQVirtualKeyboard::GetItemID()
{
    return MiscellaneousTestItem::QT_Virtual_Keyboard;
}

void MiscellaneousQVirtualKeyboard::on_btnOSK_clicked()
{
    int i = 0;
}

void MiscellaneousQVirtualKeyboard::on_btnTabtip_clicked()
{
    int i = 0;
}
