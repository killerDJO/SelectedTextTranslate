#pragma once
#include "stdafx.h"

class Utilities
{
public:
	static wchar_t* GetWideChar(string text);
	static string GetString(wchar_t* text);
	static POINT PrintText(HDC hdc, wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight);
	static void DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight);
};