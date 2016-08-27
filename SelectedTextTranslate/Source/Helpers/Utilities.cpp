#include "PrecompiledHeaders\stdafx.h"
#include "Helpers\Utilities.h"

wstring Utilities::GetUtf16String(string text)
{
	int requiredLen = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);

	wchar_t* buffer = new wchar_t[requiredLen + 1];

	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, buffer, requiredLen);

	wstring result(buffer);

	delete[] buffer;

	return result;
}

string Utilities::GetUtf8String(wstring text)
{
	int requiredLen = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, NULL, NULL);

	char* buffer = new char[requiredLen + 1];

	WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, buffer, requiredLen, NULL, NULL);

	string result(buffer);

	delete[] buffer;

	return result;
}

wchar_t* Utilities::CopyWideChar(wstring text)
{
	int len = wcslen(text.c_str());
	wchar_t* result = new wchar_t[len + 1];
	wcscpy_s(result, len + 1, text.c_str());
	return result;
}

POINT Utilities::PrintText(HDC hdc, const wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight)
{
	SelectObject(hdc, font);
	SetTextColor(hdc, color);
	
	SIZE textSize;
	GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

	TextOut(hdc, x, y, text, _tcslen(text));

	bottomRight->x = max(bottomRight->x, x + textSize.cx);
	bottomRight->y = max(bottomRight->y, y + textSize.cy);

	POINT result; 
	result.x = x + textSize.cx; 
	result.y = y + textSize.cy;
	return result;
}


void Utilities::DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight)
{
	FillRect(hdc, &rect, brush);

	bottomRight->x = max(bottomRight->x, rect.right);
	bottomRight->y = max(bottomRight->y, rect.bottom);
}