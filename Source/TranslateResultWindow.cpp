#include "stdafx.h"
#include "TranslateResultWindow.h"
#include "TranslateResult.h"

TranslateResultWindow::TranslateResultWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y) : ContentWindow(parentWindow, hInstance, x, y) { }


void TranslateResultWindow::Draw()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(this->hWindow, &ps);

	const int categoryMargin = 10;
	int curY = 0;

	HBRUSH ratingBrush = CreateSolidBrush(RGB(170, 170, 170));
	auto categories = this->translateResult.TranslateCategories;
	SetTextColor(hdc, RGB(0, 0, 0));
	for (size_t i = 0; i < categories.size(); ++i)
	{
		// Draw category header
		SIZE textSize;
		SelectObject(hdc, fontNormal);
		GetTextExtentPoint32(hdc, categories[i].BaseForm, _tcslen(categories[i].BaseForm), &textSize);
		Utilities::PrintText(hdc, categories[i].BaseForm, fontNormal, PADDING, curY);

		SetTextColor(hdc, RGB(119, 119, 119));
		Utilities::PrintText(hdc, L" - ", fontItalic, PADDING + textSize.cx + 2, curY);
		Utilities::PrintText(hdc, categories[i].PartOfSpeech, fontItalic, PADDING + textSize.cx + 17, curY);

		// Draw words
		curY += LINE_HEIGHT;
		SetTextColor(hdc, RGB(0, 0, 0));
		auto words = categories[i].Entries;
		for (size_t j = 0; j < words.size(); ++j)
		{
			Utilities::PrintText(hdc, words[j].Word, fontNormal, PADDING * 3, curY);

			int k = words[j].Score >= 0.05 ? 0 : (words[j].Score >= 0.003 ? 1 : 2);
			int rateUnit = 8;

			RECT rect;
			rect.right = PADDING + rateUnit * 3;
			rect.top = curY + LINE_HEIGHT / 3;
			rect.bottom = rect.top + LINE_HEIGHT / 3;
			rect.left = PADDING + k * rateUnit;
			FillRect(hdc, &rect, ratingBrush);
			curY += LINE_HEIGHT;
		}

		curY += categoryMargin;
	}

	EndPaint(this->hWindow, &ps);
}

TranslateResultWindow::~TranslateResultWindow()
{	
}