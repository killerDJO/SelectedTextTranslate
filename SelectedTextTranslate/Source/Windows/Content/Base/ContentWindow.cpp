#include "Windows\Content\Base\ContentWindow.h"

ContentWindow::ContentWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
	: ChildWindow(parentWindow, hInstance, x, y, width, height)
{	
	COLOR_GRAY = RGB(119, 119, 119);
	COLOR_BLACK = RGB(0, 0, 0);

	this->appModel = appModel;
}

void ContentWindow::ComputeParameters()
{
	RECT size;
	GetWindowRect(this->parentWindow, &size);

	UINT width = size.right - size.left;
	UINT height = size.bottom - size.top;

	this->kX = width / 300.0;
	this->kY = height / 400.0;

	this->LINE_HEIGHT = AdjustToResolution(20, kY);
	this->PADDING_X = AdjustToResolution(15, kX);
	this->PADDING_Y = AdjustToResolution(15, kY);
}

void ContentWindow::InitializeFonts()
{
	HDC hdc = GetDC(hWindow);

	long lfHeight = -MulDiv(FONT_HEIGHT / 2, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	long lfHeightHeader = -MulDiv(int(FONT_HEIGHT * 3 / 5.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
	long lfHeightSmall = -MulDiv(int(FONT_HEIGHT * 3 / 7.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);

	this->fontNormal = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	this->fontHeader = CreateFont(lfHeightHeader, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	this->fontItalic = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	this->fontSmall = CreateFont(lfHeightSmall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
}

void ContentWindow::InitializeBrushes()
{
	this->grayBrush = CreateSolidBrush(RGB(170, 170, 170));
}

POINT ContentWindow::RenderResult()
{
	POINT bottomRight = RenderDC();
	this->ResetWindow(bottomRight);
	InvalidateRect(this->hWindow, NULL, FALSE);
	return bottomRight;
}

ContentWindow::~ContentWindow()
{	
	DeleteObject(this->fontNormal);
	DeleteObject(this->fontHeader);
	DeleteObject(this->fontItalic);
	DeleteObject(this->fontSmall);
	DeleteObject(this->grayBrush);
}