#include "Helpers\StringUtilities.h"

wstring StringUtilities::GetUtf16String(string text)
{
    return GetUtf16StringFromChar(text.c_str());
}

wstring StringUtilities::GetUtf16StringFromChar(const char* text)
{
    if (text == nullptr)
    {
        return wstring();
    }

    int requiredLen = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);

    wchar_t* buffer = new wchar_t[requiredLen + 1];

    MultiByteToWideChar(CP_UTF8, 0, text, -1, buffer, requiredLen);

    wstring result(buffer);

    delete[] buffer;

    return result;
}

string StringUtilities::GetUtf8String(wstring text)
{
    return GetUtf8StringFromChar(text.c_str());
}

string StringUtilities::GetUtf8StringFromChar(const wchar_t* text)
{
    if(text == nullptr)
    {
        return string();
    }

    int requiredLen = WideCharToMultiByte(CP_UTF8, 0, text, -1, nullptr, 0, nullptr, nullptr);

    char* buffer = new char[requiredLen + 1];

    WideCharToMultiByte(CP_UTF8, 0, text, -1, buffer, requiredLen, nullptr, nullptr);

    string result(buffer);

    delete[] buffer;

    return result;
}

wchar_t* StringUtilities::CopyWideChar(wstring text)
{
    int len = wcslen(text.c_str());
    wchar_t* result = new wchar_t[len + 1];
    wcscpy_s(result, len + 1, text.c_str());
    return result;
}