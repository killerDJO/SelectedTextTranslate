#include "Helpers\StringUtilities.h"

wstring StringUtilities::GetUtf16String(string text)
{
	int requiredLen = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);

	wchar_t* buffer = new wchar_t[requiredLen + 1];

	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, buffer, requiredLen);

	wstring result(buffer);

	delete[] buffer;

	return result;
}

string StringUtilities::GetUtf8String(wstring text)
{
	int requiredLen = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, NULL, NULL);

	char* buffer = new char[requiredLen + 1];

	WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, buffer, requiredLen, NULL, NULL);

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