#pragma once

#include <QString>
#include <vector>

#define SL_STRINGUTIL_BUFFERLEN 1024

namespace Shuanglong::Utils
{
    enum CandidateCharacterType:uint
    {
        CAPITAL         = 0x00000001, // A-Z
        LOWERCASE       = 0x00000002, // a-z
        DIGITAL         = 0x00000004, // 0-9
        SHARP           = 0x00000008, // # hash/sharp    numbersign, 英语国家是hash，美语是pound, 音乐里作sharp, 如C# 
        ASTERISK        = 0x00000010, // * asterisk, star(美语), 数学公式中作multiply
        PLUS            = 0x00000020, // + plus/positive
        MINUS           = 0x00000040, // - minus/negative / dash   hyphen连字符, 不读
        SLASH           = 0x00000080, // / slash/slant    \ backslash
        QUOTE           = 0x00000100, // ' quote
        DOUBLE_QUOTE    = 0x00000200, // " double quote
        BACK_QUOTE      = 0x00000400, // ` backquote 反引号
        TILDE           = 0x00000800, // ~tilde
        EXCLAM          = 0x00001000, // !exclam
        AT              = 0x00002000, // @ at
        DOLLAR          = 0x00004000, // $ dollar
        PERCENT         = 0x00008000, // % percent
        CARET           = 0x00010000, // ^ caret
        AMPERSAND       = 0x00020000, // & ampersand
        PAREN           = 0x00040000, // ( paren left, opening parentheses   ) paren right, closing paretheses
        UNDERSCORE      = 0x00080000, // _ underscore
        EQUAL           = 0x00100000, // = equal 
        BRACKET         = 0x00200000, // [ bracketleft, opening bracket    ] bracketright, closing bracket
        BRACE           = 0x00400000, // { braceleft   } braceright
        SEMICOLON       = 0x00800000, // ; semicolon
        COLON           = 0x01000000, // : colon
        BAR             = 0x02000000, // | bar
        COMMA           = 0x04000000, // , comma
        LESS            = 0x08000000, // < less
        GREATER         = 0x10000000, // > greater
        PERIOD          = 0x20000000, // . period
        QUESTION        = 0x40000000, // ? question
        SPACE           = 0x80000000, // ' ' space
    };

    class StringUtil
    {
    public:
        StringUtil() = delete;
        ~StringUtil() = delete;

        static QString GetRandomPassword(uint characterTypes, int passwdLen);
        static QString Base64ShiftLeft(QString targetString, int shift);
        static QString Base64ShiftRight(QString targetString, int shift);
        static QString Base64ShiftEncode(QString targetString, int shift);
        static QString Base64ShiftDecode(QString targetString, int shift);

        std::string static FormatString(const char* format, ...);
        std::wstring static FormatWString(const wchar_t* wFormat, ...);

        std::string static FormatArguments(const char* format, va_list argList);
        std::wstring static FormatArgumentsW(const wchar_t* wFormat, va_list argList);

        std::string static Trim(const std::string& original);
        std::wstring static TrimW(const std::wstring& original);

        unsigned int static Split(std::vector<std::string>& v, std::string s, char delimiter, unsigned int maxSegments = INT_MAX);
        unsigned int static SplitW(std::vector<std::wstring>& v, std::wstring s, wchar_t delimiter, unsigned int maxSegments = INT_MAX);

        std::wstring static StringToWStringAPI(const std::string& originalStr);
        std::string static WStringToStringAPI(const std::wstring& originalStr);

        std::string static WStringToString(const std::wstring& originalStr);
        std::wstring static StringToWString(const std::string& originalStr);

        static std::string BoolToString(bool boolValue, bool bUppercase = false);
        static std::string GetHexString(const char* inBuffer, size_t inLength, bool bUpperCase = true);
        static std::string GetHexString(const char* inBuffer, size_t inLength, const char* prefix, const char* postfix, bool bUpperCase = true);

    private:
        static QByteArray MakeCandidateCharacterArray(uint characterTypes);
        static int Base64CharToIndex(char targetChar);
        static char Base64IndexToChar(int targetIndex);
        static char GetHexChar(const char charByte, bool bUpperCase);
    };
}

