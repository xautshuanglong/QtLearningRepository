#include "StringUtil.h"


QString StringUtil::GetRandomPassword(CandidateCharacterType characterTypes, int passwdLen)
{
    QByteArray passwdBytes;
    for (int i = 0; i < passwdLen; ++i)
    {
        passwdBytes.append('x');
    }
    return QString(passwdBytes);
}

QString StringUtil::CharacterShiftRight(QString targetString, int shift)
{
    return QString();
}

QString StringUtil::CharacterShiftLeft(QString targetString, int shift)
{
    return QString();
}

QString StringUtil::Base64ShiftEncode(QString targetString)
{
    return QString();
}

QString StringUtil::Base64ShiftDecode(QString targetString)
{
    return QString();
}
