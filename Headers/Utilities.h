#pragma once
#include "stdafx.h"

class Utilities
{
public:
	static wchar_t* GetWideChar(string text);
	static string GetString(wchar_t* text);
	static void PrintText(HDC hdc, wchar_t* text, HFONT font, int x, int y);
};