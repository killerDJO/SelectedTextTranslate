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

wstring StringUtilities::Format(const wstring format, ...) {
    va_list args;

    va_start(args, format);
    wstring result = Format(format, args);
    va_end(args);

    return result;
}

wstring StringUtilities::Format(const wstring format, va_list args) {
    int final_n, n = (int)format.size() * 2; /* Reserve two times as much as the length of the format */
    std::wstring str;
    std::unique_ptr<wchar_t[]> formatted;

    while (1) {
        formatted.reset(new wchar_t[n]); /* Wrap the plain char array into the unique_ptr */
        wcscpy(&formatted[0], format.c_str());

        final_n = _vsnwprintf(&formatted[0], n, format.c_str(), args);

        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }

    return wstring(formatted.get());
}