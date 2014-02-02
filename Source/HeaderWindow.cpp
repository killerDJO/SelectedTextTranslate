#include "stdafx.h"
#include "HeaderWindow.h"

HeaderWindow::HeaderWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y) : ContentWindow(parentWindow, hInstance, x, y, 1000, 50) 
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


POINT HeaderWindow::RenderDC()
{
	ContentWindow::RenderDC();

	POINT bottomRight = { 0, 0 };

	int curY = PADDING / 2;

	Utilities::PrintText(this->inMemoryHDC, translateResult.Sentence.Translation, fontHeader, COLOR_BLACK, PADDING, curY, &bottomRight);
	Utilities::PrintText(this->inMemoryHDC, translateResult.Sentence.Origin, fontSmall, COLOR_GRAY, PADDING + 17, curY + 20, &bottomRight);
	curY += 13;

	RECT rect;
	rect.right = 0;
	rect.left = 1000;
	rect.top = curY + int(5 / 4.0*LINE_HEIGHT);
	rect.bottom = rect.top + 1;
	Utilities::DrawRect(this->inMemoryHDC, rect, this->grayBrush, &bottomRight);

	return bottomRight;
}

HeaderWindow::~HeaderWindow()
{	

}