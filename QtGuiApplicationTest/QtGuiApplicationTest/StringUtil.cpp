#include "StringUtil.h"

//#include <time.h>
#include <windows.h>
#include <stdlib.h>

#include <LogUtil.h>

QString StringUtil::GetRandomPassword(uint characterTypes, int passwdLen)
{
    static int randSeed = 0;
    int newSeed = ::GetTickCount();
    if (randSeed != newSeed)
    {
        randSeed = newSeed;
        srand(randSeed);
    }
    QByteArray passwdBytes;
    QByteArray characterArray = StringUtil::MakeCandidateCharacterArray(characterTypes);
    for (int i = 0; i < passwdLen; ++i)
    {
        int randNum;
        int randTimes = rand() % 10 + 1;
        for (int j = 0; j < randTimes; ++j)
        {
            randNum = rand();
        }
        char character = 0;
        if (characterArray.count() != 0)
        {
            int index = randNum % characterArray.count();
            character = characterArray[index];
        }
        //LogUtil::Debug(CODE_LOCATION, "randNum = %d     character = %c", randNum, character);
        passwdBytes.append(character);
    }
    return QString(passwdBytes);
}

QString StringUtil::Base64ShiftLeft(QString targetString, int shift)
{
    QString retValue;
    int oldIndex = 0, newIndex = 0;
    int strLen = targetString.length();
    int realShift = shift;
    if (realShift > 0)
    {
        realShift = realShift % 64;
    }
    else
    {
        while (realShift < 0)
        {
            realShift += 64;
        }
    }
    for (int i = 0; i < strLen; ++i)
    {
        oldIndex = StringUtil::Base64CharToIndex(targetString.at(i).toLatin1());
        newIndex = oldIndex < realShift ? oldIndex + 64 - realShift : oldIndex - realShift;
        retValue.append(StringUtil::Base64IndexToChar(newIndex));
    }
    return retValue;
}

QString StringUtil::Base64ShiftRight(QString targetString, int shift)
{
    QString retValue;
    int oldIndex = 0, newIndex = 0;
    int strLen = targetString.length();
    int realShift = shift;
    if (realShift > 0)
    {
        realShift = realShift % 64;
    }
    else
    {
        while (realShift < 0)
        {
            realShift += 64;
        }
    }
    for (int i = 0; i < strLen; ++i)
    {
        oldIndex = StringUtil::Base64CharToIndex(targetString.at(i).toLatin1());
        newIndex = oldIndex > 64 - realShift ? oldIndex + realShift -64 : oldIndex + realShift;
        retValue.append(StringUtil::Base64IndexToChar(newIndex));
    }
    return retValue;
}

QString StringUtil::Base64ShiftEncode(QString targetString, int shift)
{
    QString base64 = targetString.toLatin1().toBase64();
    QString retValue = StringUtil::Base64ShiftRight(base64, shift);
    return retValue;
}

QString StringUtil::Base64ShiftDecode(QString targetString, int shift)
{
    QString base64 = StringUtil::Base64ShiftLeft(targetString, shift);
    QString retValue = QByteArray::fromBase64(base64.toLatin1());
    return retValue;
}

QByteArray StringUtil::MakeCandidateCharacterArray(uint characterTypes)
{
    QByteArray characterArray;
    int characterType = 0;
    if (characterTypes != 0)
    {
        for (int i = 0; i < 32; ++i)
        {
            characterType = characterTypes & (1 << i);
            switch (characterType)
            {
            case CAPITAL:
                for (int i = 0; i < 26; ++i)
                {
                    characterArray.append('A' + i);
                }
                break;
            case LOWERCASE:
                for (int i = 0; i < 26; ++i)
                {
                    characterArray.append('a' + i);
                }
                break;
            case DIGITAL:
                for (int i = 0; i < 10; ++i)
                {
                    characterArray.append('0' + i);
                }
                break;
            case SHARP:
                characterArray.append('#');
                break;
            case ASTERISK:
                characterArray.append('*');
                break;
            case PLUS:
                characterArray.append('+');
                break;
            case MINUS:
                characterArray.append('-');
                break;
            case SLASH:
                characterArray.append('/');
                characterArray.append('\\');
                break;
            case QUOTE:
                characterArray.append('\'');
                break;
            case DOUBLE_QUOTE:
                characterArray.append('"');
                break;
            case BACK_QUOTE:
                characterArray.append('`');
                break;
            case TILDE:
                characterArray.append('`');
                break;
            case EXCLAM:
                characterArray.append('!');
                break;
            case AT:
                characterArray.append('@');
                break;
            case DOLLAR:
                characterArray.append('$');
                break;
            case PERCENT:
                characterArray.append('%');
                break;
            case CARET:
                characterArray.append('^');
                break;
            case AMPERSAND:
                characterArray.append('&');
                break;
            case PAREN:
                characterArray.append('(');
                characterArray.append(')');
                break;
            case UNDERSCORE:
                characterArray.append('_');
                break;
            case EQUAL:
                characterArray.append('=');
                break;
            case BRACKET:
                characterArray.append('[');
                characterArray.append(']');
                break;
            case BRACE:
                characterArray.append('{');
                characterArray.append('}');
                break;
            case SEMICOLON:
                characterArray.append(';');
                break;
            case COLON:
                characterArray.append(':');
                break;
            case BAR:
                characterArray.append('|');
                break;
            case COMMA:
                characterArray.append(',');
                break;
            case LESS:
                characterArray.append('<');
                break;
            case GREATER:
                characterArray.append('>');
                break;
            case PERIOD:
                characterArray.append('.');
                break;
            case QUESTION:
                characterArray.append('?');
                break;
            case SPACE:
                characterArray.append(' ');
                break;
            default:
                break;
            }
        }
    }

    return characterArray;
}

int StringUtil::Base64CharToIndex(char targetChar)
{
    int retValue = 0;
    if ('A' <= targetChar && targetChar <= 'Z')
    {
        retValue = targetChar - 'A';
    }
    else if ('a' <= targetChar && targetChar <= 'z')
    {
        retValue = targetChar - 'a' + 26;
    }
    else if ('0' <= targetChar && targetChar <= '9')
    {
        retValue = targetChar - '0' + 52;
    }
    else if ('+' == targetChar)
    {
        retValue = 62;
    }
    else if ('-' == targetChar)
    {
        retValue = 63;
    }
    else if ('=' == targetChar)
    {
        retValue = 64;
    }
    return retValue;
}

char StringUtil::Base64IndexToChar(int targetIndex)
{
    char retValue = 0;
    if (0 <= targetIndex && targetIndex <= 25)
    {
        retValue = 'A' + targetIndex;
    }
    else if (26 <= targetIndex && targetIndex <= 51)
    {
        retValue = 'a' + (targetIndex - 26);
    }
    else if (52 <= targetIndex && targetIndex <= 61)
    {
        retValue = '0' + (targetIndex - 52);
    }
    else if (62 == targetIndex)
    {
        retValue = '+';
    }
    else if (63 == targetIndex)
    {
        retValue = '-';
    }
    else if (64 == targetIndex)
    {
        retValue = '=';
    }
    return retValue;
}
