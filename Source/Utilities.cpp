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

POINT Utilities::PrintText(HDC hdc, wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight)
{
	SelectObject(hdc, font);
	SetTextColor(hdc, color);
	
	SIZE textSize;
	GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

	TextOut(hdc, x, y, text, _tcslen(text));

	bottomRight->x = max(bottomRight->x, x + textSize.cx);
	bottomRight->y = max(bottomRight->y, y + textSize.cy);

	return { x + textSize.cx, y + textSize.cy };
}


void Utilities::DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight)
{
	FillRect(hdc, &rect, brush);

	bottomRight->x = max(bottomRight->x, rect.right);
	bottomRight->y = max(bottomRight->y, rect.bottom);
}