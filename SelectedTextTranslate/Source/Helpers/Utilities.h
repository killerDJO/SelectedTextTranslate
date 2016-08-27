#pragma once
#include "PrecompiledHeaders\stdafx.h"

class Utilities
{
public:
	static wstring GetUtf16String(string text);
	static string GetUtf8String(wstring text);
	static wchar_t*  CopyWideChar(wstring text);

	static POINT PrintText(HDC hdc, const wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight);
	static void DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight);
};