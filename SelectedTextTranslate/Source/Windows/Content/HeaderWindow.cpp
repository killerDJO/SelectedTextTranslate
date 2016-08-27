#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Content\HeaderWindow.h"

HeaderWindow::HeaderWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height) 
: ContentWindow(parentWindow, hInstance, x, y, width, height) 
{ 
	this->InitializeAudioButton();
	SetWindowLongPtr(this->hWindow, GWL_WNDPROC, (LONG_PTR)HeaderWindow::WndProc);
}

void HeaderWindow::InitializeAudioButton()
{
	DWORD imageSize = 15;
	DWORD adjustedSize = AdjustToResolution(15, kY);
	audioButton = new AudioButtonWindow(
		this->hWindow, 
		this->hInstance, 
		PADDING_X,
		PADDING_Y / 2 + AdjustToResolution(19, kY) + (adjustedSize - imageSize)/2,
		imageSize,
		imageSize);
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

	int curY = PADDING_Y / 2;

	Utilities::PrintText(this->inMemoryHDC, translateResult.Sentence.Translation, fontHeader, COLOR_BLACK, PADDING_X, curY, &bottomRight);
	Utilities::PrintText(this->inMemoryHDC, translateResult.Sentence.Origin, fontSmall, COLOR_GRAY, PADDING_X + AdjustToResolution(16, kY) - int((kY-1)*10), curY + AdjustToResolution(20, kY), &bottomRight);

	RECT rect;
	rect.left = 0;
	rect.right = 2000;
	rect.top = curY + 2 * LINE_HEIGHT;
	rect.bottom = rect.top + 1;
	Utilities::DrawRect(this->inMemoryHDC, rect, this->grayBrush, &POINT());

	bottomRight.x += PADDING_X * 3;
	bottomRight.y = rect.bottom;

	return bottomRight;
}

void HeaderWindow::ResetWindow(POINT bottomRight)
{
	if (wcslen(this->translateResult.Sentence.Origin) != 0)
	{
		this->audioButton->Show();
	}
	else 
	{
		this->audioButton->Hide();
	}
	MoveWindow(this->hWindow, this->initialX, this->initialY, 2000, bottomRight.y, FALSE);
}

HeaderWindow::~HeaderWindow()
{	
}

POINT HeaderWindow::RenderResult(TranslateResult translateResult)
{
	this->translateResult = translateResult;
	return ContentWindow::RenderResult();
}
