#include "Utilities\StringUtilities.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

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
    AssertCriticalWinApiResult(requiredLen);

    wchar_t* buffer = new wchar_t[requiredLen + 1];

    AssertCriticalWinApiResult(MultiByteToWideChar(CP_UTF8, 0, text, -1, buffer, requiredLen));

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
    AssertCriticalWinApiResult(requiredLen);

    char* buffer = new char[requiredLen + 1];

    AssertCriticalWinApiResult(WideCharToMultiByte(CP_UTF8, 0, text, -1, buffer, requiredLen, nullptr, nullptr));

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
    size_t requiredLength = format.length();
    unique_ptr<wchar_t[]> formatted;
    int truncationResult;

    do
    {
        requiredLength = requiredLength * 2;
        formatted.reset(new wchar_t[requiredLength]);
        wcscpy_s(&formatted[0], requiredLength, format.c_str());

        truncationResult = _vsnwprintf_s(&formatted[0], requiredLength, _TRUNCATE, format.c_str(), args);
    }
    while (truncationResult == -1);

    return wstring(formatted.get());
}

vector<wstring> StringUtilities::Split(const wstring &s, wchar_t delim)
{
    vector<wstring> elems;

    wstringstream ss(s);
    wstring item;
    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

void StringUtilities::ReplaceAll(wstring &str, const wstring &search, const wstring &replace)
{
    for (size_t pos = 0;; pos += replace.length() - 1)
    {
        pos = str.find(search, pos);
        if (pos == string::npos) break;

        str.erase(pos, search.length());
        str.insert(pos, replace);
    }
}

wstring StringUtilities::LeftTrim(wstring str)
{
    str.erase(str.begin(), find_if(str.begin(), str.end(), not1(std::ptr_fun<int, int>(std::isspace))));
    return str;
}

wstring StringUtilities::RightTrim(wstring str)
{
    str.erase(find_if(str.rbegin(), str.rend(), not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
    return str;
}

wstring StringUtilities::Trim(wstring str)
{
    return LeftTrim(RightTrim(str));
}