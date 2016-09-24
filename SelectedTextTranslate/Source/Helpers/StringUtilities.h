#pragma once

class StringUtilities
{
public:
    static wstring GetUtf16String(string text);
    static wstring GetUtf16StringFromChar(const char* text);

    static string GetUtf8String(wstring text);
    static string GetUtf8StringFromChar(const wchar_t* text);

    static wchar_t*  CopyWideChar(wstring text);
};