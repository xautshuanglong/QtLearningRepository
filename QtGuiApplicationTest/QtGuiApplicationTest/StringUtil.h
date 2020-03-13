#pragma once

#include <QString>

enum CandidateCharacterType
{
    CAPITAL   = 0x00000001,
    LOWERCASE = 0x00000002,
    DIGITAL   = 0x00000004
};

class StringUtil
{
public:
    StringUtil() = delete;
    ~StringUtil() = delete;

    static QString GetRandomPassword(CandidateCharacterType characterTypes, int passwdLen);
    static QString CharacterShiftRight(QString targetString, int shift);
    static QString CharacterShiftLeft(QString targetString, int shift);
    static QString Base64ShiftEncode(QString targetString);
    static QString Base64ShiftDecode(QString targetString);
};
