#pragma once

class StringUtilities
{
public:
    static wstring GetUtf16String(string text);
    static string GetUtf8String(wstring text);
    static wchar_t*  CopyWideChar(wstring text);
};