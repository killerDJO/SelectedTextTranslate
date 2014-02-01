#include "stdafx.h"
#include "TranslateResultWindow.h"
#include "TranslateResult.h"

TranslateResultWindow::TranslateResultWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y) : ContentWindow(parentWindow, hInstance, x, y) { }

void TranslateResultWindow::RenderResult(TranslateResult translateResult)
{
	printf("RenderResult\n");
	this->translateResult = translateResult;
	/////////////
	//work
	this->inMemoryHDC = CreateCompatibleDC(GetDC(this->hWindow));
	BITMAPINFO i;
	ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
	i.bmiHeader.biWidth = WINDOW_WIDTH; 
	i.bmiHeader.biHeight = WINDOW_HEIGHT;   
	i.bmiHeader.biPlanes = 1;
	i.bmiHeader.biBitCount = 24; 
	i.bmiHeader.biSizeImage = 0;
	i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	i.bmiHeader.biClrUsed = 0;
	i.bmiHeader.biClrImportant = 0;
	VOID *pvBits;
	HBITMAP bitmap = CreateDIBSection(this->inMemoryHDC,&i,DIB_RGB_COLORS,&pvBits,NULL,0);

	//HBITMAP bitmap = CreateCompatibleBitmap(inMemoryHDC, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(this->inMemoryHDC, bitmap);

	RECT rect; rect.top = 0; rect.left = 0; rect.bottom = WINDOW_HEIGHT; rect.right = WINDOW_WIDTH;
	FillRect(inMemoryHDC, &rect, CreateSolidBrush(RGB(255, 255, 255)));

	const int categoryMargin = 10;
	int curY = 0;

	HBRUSH ratingBrush = CreateSolidBrush(RGB(170, 170, 170));
	auto categories = this->translateResult.TranslateCategories;
	SetTextColor(inMemoryHDC, RGB(0, 0, 0));
	for (size_t i = 0; i < categories.size(); ++i)
	{
		// Draw category header
		SIZE textSize;
		SelectObject(inMemoryHDC, fontNormal);
		GetTextExtentPoint32(inMemoryHDC, categories[i].BaseForm, _tcslen(categories[i].BaseForm), &textSize);
		Utilities::PrintText(inMemoryHDC, categories[i].BaseForm, fontNormal, PADDING, curY);

		SetTextColor(inMemoryHDC, RGB(119, 119, 119));
		if (_tcslen(categories[i].PartOfSpeech) != 0)
		{
			Utilities::PrintText(inMemoryHDC, L" - ", fontItalic, PADDING + textSize.cx + 2, curY);
			Utilities::PrintText(inMemoryHDC, categories[i].PartOfSpeech, fontItalic, PADDING + textSize.cx + 17, curY);
		}

		// Draw words
		curY += LINE_HEIGHT;
		SetTextColor(inMemoryHDC, RGB(0, 0, 0));
		auto words = categories[i].Entries;
		for (size_t j = 0; j < words.size(); ++j)
		{
			Utilities::PrintText(inMemoryHDC, words[j].Word, fontNormal, PADDING * 3, curY);

			int k = words[j].Score >= 0.05 ? 0 : (words[j].Score >= 0.003 ? 1 : 2);
			int rateUnit = 8;

			RECT rect;
			rect.right = PADDING + rateUnit * 3;
			rect.top = curY + LINE_HEIGHT / 3;
			rect.bottom = rect.top + LINE_HEIGHT / 3;
			rect.left = PADDING + k * rateUnit;
			FillRect(inMemoryHDC, &rect, ratingBrush);
			curY += LINE_HEIGHT;
		}

		curY += categoryMargin;
	}
	///////////////

	MoveWindow(this->hWindow, this->initialX, this->initialY, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
	InvalidateRect(this->hWindow, NULL, FALSE);
	this->Draw();
}

void TranslateResultWindow::Draw()
{
	printf("Draw\n");
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(this->hWindow, &ps);
	DWORD res = BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, this->inMemoryHDC, 0, 0, SRCCOPY);
	//SelectObject(hdc, this->renderedResult);

	EndPaint(this->hWindow, &ps);
}

TranslateResultWindow::~TranslateResultWindow()
{	
}