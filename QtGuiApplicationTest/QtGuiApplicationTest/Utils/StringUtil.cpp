#include "StringUtil.h"

//#include <time.h>
#include <windows.h>
#include <stdlib.h>

#include "JCB_Logger/LogUtil.h"

#define BASE64_COUNT 65 // A-Z a-z 0-9 + - =

namespace Shuanglong::Utils
{
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
        if (realShift > BASE64_COUNT)
        {
            realShift = realShift % BASE64_COUNT;
        }
        while (realShift < 0)
        {
            realShift += BASE64_COUNT;
        }
        for (int i = 0; i < strLen; ++i)
        {
            oldIndex = StringUtil::Base64CharToIndex(targetString.at(i).toLatin1());
            newIndex = oldIndex >= realShift ? oldIndex - realShift : oldIndex + BASE64_COUNT - realShift;
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
        if (realShift >= BASE64_COUNT)
        {
            realShift = realShift % BASE64_COUNT;
        }
        while (realShift < 0)
        {
            realShift += BASE64_COUNT;
        }
        for (int i = 0; i < strLen; ++i)
        {
            oldIndex = StringUtil::Base64CharToIndex(targetString.at(i).toLatin1());
            newIndex = oldIndex + realShift < BASE64_COUNT ? oldIndex + realShift : oldIndex + realShift - BASE64_COUNT;
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

    std::string StringUtil::FormatString(const char* format, ...)
    {
        va_list argList;
        va_start(argList, format);
        std::string retString = FormatArguments(format, argList);
        va_end(argList);

        return retString;
    }

    std::string StringUtil::FormatArguments(const char* format, va_list argList)
    {
        std::string retString;
        char buffer[SL_STRINGUTIL_BUFFERLEN];
        _vsnprintf_s(buffer, SL_STRINGUTIL_BUFFERLEN, _TRUNCATE, format, argList);
        retString = buffer;

        return retString;
    }

    std::wstring StringUtil::FormatWString(const wchar_t* wFormat, ...)
    {
        va_list argList;
        va_start(argList, wFormat);
        std::wstring retString = FormatArgumentsW(wFormat, argList);
        va_end(argList);

        return retString;
    }

    std::wstring StringUtil::FormatArgumentsW(const wchar_t* wFormat, va_list argList)
    {
        std::wstring retString;
        wchar_t buffer[SL_STRINGUTIL_BUFFERLEN];
        _vsnwprintf_s(buffer, SL_STRINGUTIL_BUFFERLEN, _TRUNCATE, wFormat, argList);
        retString = buffer;

        return retString;
    }

    std::string StringUtil::Trim(const std::string& original)
    {
        static const char* whiteSpace = " \t\r\n";
        if (original.empty() || original.length() == 0)
        {
            return original;
        }
        std::string::size_type begin = original.find_first_not_of(whiteSpace);
        if (begin == std::string::npos)
        {
            return "";
        }
        std::string::size_type end = original.find_last_not_of(whiteSpace);
        return std::string(original, begin, end - begin + 1);
    }

    std::wstring StringUtil::TrimW(const std::wstring& original)
    {
        static const wchar_t* whiteSpace = L" \t\r\n";
        if (original.empty() || original.length() == 0)
        {
            return original;
        }
        std::string::size_type begin = original.find_first_not_of(whiteSpace);
        if (begin == std::string::npos)
        {
            return L"";
        }
        std::wstring::size_type end = original.find_last_not_of(whiteSpace);
        return std::wstring(original, begin, end - begin + 1);
    }

    unsigned int StringUtil::Split(std::vector<std::string>& v, std::string s, char delimiter, unsigned int maxSegments /* = INT_MAX */)
    {
        unsigned int i = 0;
        std::string::size_type left = 0, right = 0;
        v.clear();
        std::back_insert_iterator<std::vector<std::string>> it(v);
        for (i = 1; i < maxSegments; i++)
        {
            right = s.find(delimiter, left);
            if (right == std::string::npos)
            {
                break;
            }
            *it++ = s.substr(left, right - left);
            left = right + 1;
        }
        *it++ = s.substr(left);
        return i;
    }

    unsigned int StringUtil::SplitW(std::vector<std::wstring>& v, std::wstring s, wchar_t delimiter, unsigned int maxSegments /* = INT_MAX */)
    {
        unsigned int i = 0;
        std::wstring::size_type left = 0, right = 0;
        v.clear();
        std::back_insert_iterator<std::vector<std::wstring>> it(v);
        for (i = 1; i < maxSegments; i++)
        {
            right = s.find(delimiter, left);
            if (right == std::wstring::npos)
            {
                break;
            }
            *it++ = s.substr(left, right - left);
            left = right + 1;
        }
        *it++ = s.substr(left);
        return i;
    }

    std::wstring StringUtil::StringToWStringAPI(const std::string& originalStr)
    {
        wchar_t* pwChar = nullptr;
        std::wstring retString = L"";
        LPCSTR lpStr = originalStr.c_str();
        int nLen = MultiByteToWideChar(CP_ACP, 0, lpStr, -1, NULL, 0);
        if (nLen == 0)
        {
            return std::wstring(L"");
        }

        try
        {
            pwChar = new wchar_t[nLen];
        }
        catch (const std::bad_alloc& e)
        {
            const char* pErrMsg = e.what();
            printf(pErrMsg);
            return std::wstring(L"");
        }

        MultiByteToWideChar(CP_ACP, 0, lpStr, -1, pwChar, nLen);
        retString = pwChar;
        delete[] pwChar;

        return retString;
    }

    std::string StringUtil::WStringToStringAPI(const std::wstring& originalStr)
    {
        std::string retString = "";
        char* pChar = nullptr;
        LPCWSTR lpwStr = originalStr.c_str();
        int nLen = WideCharToMultiByte(CP_ACP, 0, lpwStr, -1, NULL, 0, NULL, NULL);
        if (nLen == 0)
        {
            return std::string("");
        }

        try
        {
            pChar = new char[nLen];
        }
        catch (const std::bad_alloc& e)
        {
            const char* pErrMsg = e.what();
            printf(pErrMsg);
            return std::string("");
        }

        WideCharToMultiByte(CP_ACP, 0, lpwStr, -1, pChar, nLen, NULL, NULL);
        retString = pChar;
        delete[] pChar;

        return retString;
    }

    std::string StringUtil::WStringToString(const std::wstring& originalStr)
    {
        // (since C++11)
        // (deprecated in C++17)
        //std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> cvtAnsi(new std::codecvt<wchar_t, char, std::mbstate_t>("chs"));
        //std::string retString = cvtAnsi.to_bytes(originalStr);

        std::string retString = StringUtil::WStringToStringAPI(originalStr);

        // UTF-8 ? UNICODE
        //std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
        //std::wstring tempString = convert.from_bytes("\xe4\xb8\xad\xe6\x96\x87");
        //std::wcout << tempString << std::endl;

        return retString;
    }

    std::wstring StringUtil::StringToWString(const std::string& originalStr)
    {
        // (since C++11)
        // (deprecated in C++17)
        //std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> cvtAnsi(new std::codecvt<wchar_t, char, std::mbstate_t>("chs"));
        //std::wstring retString = cvtAnsi.from_bytes(originalStr);

        std::wstring retString = StringUtil::StringToWStringAPI(originalStr);

        return retString;
    }

    std::string StringUtil::BoolToString(bool boolValue, bool bUppercase /* = false */)
    {
        std::string retValue = "false";
        if (boolValue)
        {
            retValue = "true";
        }

        if (bUppercase)
        {
            std::transform(retValue.begin(), retValue.end(), retValue.begin(), toupper);
        }

        return retValue;
    }

    /************************************************************************\
        Description: 将传入的字节串（可能含有不可见字符）
        转换成十六进制表示的字符串（不插入分隔符）
        Arguments  :
        inBuffer --> 需要转换的字节串
        inLength --> 传入字节串的长度（不含终结符）
        bUpperCase --> 指明字母用大小还是小写方式返回
    \************************************************************************/
    std::string StringUtil::GetHexString(const char* inBuffer, size_t inLength, bool bUpperCase /* = true */)
    {
        std::string retString = "";
        for (size_t i = 0; i < inLength; ++i)
        {
            retString += GetHexChar(inBuffer[i] >> 4 & 0x0F, bUpperCase);
            retString += GetHexChar(inBuffer[i] & 0x0F, bUpperCase);
        }

        return retString;
    }

    /************************************************************************\
        Description: 将传入的字节串（可能含有不可见字符）
        转换成十六进制表示的字符串（可插入前缀及后缀）
        Arguments  :
        inBuffer   --> 需要转换的字节串
        inLength   --> 传入字节串的长度（不含终结符）
        prefix     --> 每个十六进制前面的符号
        postfix    --> 每个十六进制后面的符号
        bUpperCase --> 指明字母用大小还是小写方式返回
    \************************************************************************/
    std::string StringUtil::GetHexString(const char* inBuffer, size_t inLength, const char* prefix, const char* postfix, bool bUpperCase /* = true */)
    {
        std::string retString = "";
        for (size_t i = 0; i < inLength; ++i)
        {
            retString += prefix;
            retString += GetHexChar(inBuffer[i] >> 4 & 0x0F, bUpperCase);
            retString += GetHexChar(inBuffer[i] & 0x0F, bUpperCase);
            retString += postfix;
        }

        return retString;
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

    /************************************************************************\
    Description: 将单个字节转成字符
    Arguments  :
    charByte --> 需要转化的字节值
    bUpperCase --> 指明字母用大小还是小写方式返回
    \************************************************************************/
    char StringUtil::GetHexChar(const char charByte, bool bUpperCase)
    {
        char startChar = 'A';
        char retChar = '\0';
        if (charByte < 10)
        {
            retChar = '0' + charByte;
        }
        else
        {
            if (!bUpperCase)
            {
                startChar = 'a';
            }
            retChar = startChar - 10 + charByte;
        }

        return retChar;
    }
}
