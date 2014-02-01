#include "stdafx.h"
#include "Utilities.h"

wchar_t* Utilities::GetWideChar(string text)
{
	static wchar_t buffer[65536];

	int len = MultiByteToWideChar(20866, 0, text.c_str(), -1, buffer, 65535);
	wchar_t* result = new wchar_t[len];
	std::copy(buffer, buffer + len, result);
	return result;
}

string Utilities::GetString(wchar_t* text)
{
	static char buffer[65536];
	int len = WideCharToMultiByte(20866, 0, text, -1, buffer, 65535, NULL, NULL);
	return string(buffer);
}

void Utilities::PrintText(HDC hdc, wchar_t* text, HFONT font, int x, int y)
{
	SelectObject(hdc, font);
	TextOut(hdc, x, y, text, _tcslen(text));
}