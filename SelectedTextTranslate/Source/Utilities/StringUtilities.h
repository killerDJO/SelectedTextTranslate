#pragma once

class StringUtilities
{
public:
    static wstring GetUtf16String(string text);
    static wstring GetUtf16StringFromChar(const char* text);

    static string GetUtf8String(wstring text);
    static string GetUtf8StringFromChar(const wchar_t* text);

    static wchar_t*  CopyWideChar(wstring text);

    static wstring Format(const wstring format, ...);
    static wstring Format(const wstring format, va_list args);

    static vector<wstring> Split(const wstring &s, wchar_t delim);
    static void ReplaceAll(wstring &str, const wstring &search, const wstring &replace);
};