#include "MiscellaneousStringUtil.h"

#include <StringUtil.h>

MiscellaneousStringUtil::MiscellaneousStringUtil(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousStringUtil::~MiscellaneousStringUtil()
{
}

QString MiscellaneousStringUtil::GetTitle()
{
    return QObject::tr("StringUtil");
}

QString MiscellaneousStringUtil::GetTitleTooltip()
{
    return QObject::tr("StringUtil methods test entry.");
}

MiscellaneousTestGroup MiscellaneousStringUtil::GetGroupID()
{
    return MiscellaneousTestGroup::Utility_Tools;
}

MiscellaneousTestItem MiscellaneousStringUtil::GetItemID()
{
    return MiscellaneousTestItem::Utility_StringUtil;
}

void MiscellaneousStringUtil::on_btnCharacterShiftLeft_clicked()
{
    QString target = ui.leShiftTarget->text();
    m_strShifted = StringUtil::CharacterShiftLeft(target, 5);
}

void MiscellaneousStringUtil::on_btnCharacterShiftRight_clicked()
{
    QString original = StringUtil::CharacterShiftRight(m_strShifted, 5);
}

void MiscellaneousStringUtil::on_btnBase64ShiftEncode_clicked()
{
    QString target = ui.leEncodeTarget->text();
    QString m_strEncoded = StringUtil::Base64ShiftEncode(target);
}

void MiscellaneousStringUtil::on_btnBase64ShiftDecode_clicked()
{
    QString original = StringUtil::Base64ShiftDecode(m_strEncoded);
}

void MiscellaneousStringUtil::on_btnRandomPassword_clicked()
{
    QString password = StringUtil::GetRandomPassword(CAPITAL, 5);
    ui.leEncodeTarget->setText(password);
    ui.leShiftTarget->setText(password);
}
