#include "stdafx.h"
#include "HeaderWindow.h"

HeaderWindow::HeaderWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y) : ContentWindow(parentWindow, hInstance, x, y) 
{ 
	this->InitializeAudioButton();
	SetWindowLongPtr(this->hWindow, GWL_WNDPROC, (LONG_PTR)HeaderWindow::WndProc);
}

void HeaderWindow::InitializeAudioButton()
{
	audioButton = new AudioButtonWindow(this->hWindow, this->hInstance, 15, 26);
}

LRESULT CALLBACK HeaderWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HeaderWindow* instance = (HeaderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
		case WM_COMMAND:
			if (LOWORD(wParam) == BN_CLICKED)
			{
				instance->PlayText();
				return TRUE;
			}
			break;
	}
	return ContentWindow::WndProc(hWnd, message, wParam, lParam);
}

void HeaderWindow::PlayText()
{
	TextPlayer::PlayText(this->translateResult.Sentence.Origin);
}

void HeaderWindow::Draw()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(this->hWindow, &ps);

	const int categoryMargin = 10;
	int curY = PADDING / 2;

	HBRUSH ratingBrush = CreateSolidBrush(RGB(170, 170, 170));

	Utilities::PrintText(hdc, this->translateResult.Sentence.Translation, fontHeader, PADDING, curY);

	SetTextColor(hdc, RGB(119, 119, 119));
	Utilities::PrintText(hdc, this->translateResult.Sentence.Origin, fontSmall, PADDING + 17, curY + 20);
	curY += 13;

	RECT rect;
	rect.right = 0;
	rect.left = 1000;
	rect.top = curY + int(5 / 4.0*LINE_HEIGHT);
	rect.bottom = rect.top + 1;
	FillRect(hdc, &rect, ratingBrush);

	EndPaint(this->hWindow, &ps);
}

HeaderWindow::~HeaderWindow()
{	
}