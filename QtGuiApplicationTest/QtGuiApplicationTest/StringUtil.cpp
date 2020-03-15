#include "StringUtil.h"

//#include <time.h>
#include <windows.h>
#include <stdlib.h>

#include <LogUtil.h>

QString StringUtil::GetRandomPassword(CandidateCharacterType characterTypes, int passwdLen)
{
    static int randSeed = 0;
    int newSeed = ::GetTickCount();
    if (randSeed != newSeed)
    {
        randSeed = newSeed;
        srand(randSeed);
    }
    QByteArray passwdBytes;
    for (int i = 0; i < passwdLen; ++i)
    {
        int randNum = rand();
        LogUtil::Debug(CODE_LOCATION, "Rand number : %d", randNum);

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
