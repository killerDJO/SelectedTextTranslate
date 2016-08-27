#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Buttons\ExpandButtonWindow.h"

ExpandButtonWindow::ExpandButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height, DWORD hiddenCount, bool expandedState)
{
	this->hInstance = hInstance;
	this->hWindow = CreateWindowA(
		"STATIC",
		NULL,
		WS_CHILD | WS_VISIBLE | SS_REALSIZECONTROL | SS_LEFT | SS_NOTIFY,
		x,
		y,
		width,
		height,
		parentWindow,
		NULL,
		this->hInstance,
		NULL);

	InitializeFonts();

	SetWindowSubclass(this->hWindow, ExpandButtonWindow::WndProc, 0, (DWORD)this);
	
	SendMessage(this->hWindow, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));

	wstring text = L"";
	if (!expandedState){
		if (hiddenCount == 1){
			text = L"show " + to_wstring(hiddenCount) + L" more result";
		}
		else {
			text = L"show " + to_wstring(hiddenCount) + L" more results";
		}
	}
	else {
		text = L"show less results";
	}
	DWORD adjustedWidth = GetTextWidth(text);

	MoveWindow(this->hWindow, x, y, adjustedWidth, height, TRUE);
	
	SetWindowText(this->hWindow, text.c_str());
}

DWORD ExpandButtonWindow::GetTextWidth(wstring text)
{
	HDC fakeDC = CreateCompatibleDC(NULL);

	BITMAPINFO i;
	ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
	i.bmiHeader.biWidth = 2000;
	i.bmiHeader.biHeight = 50;
	i.bmiHeader.biPlanes = 1;
	i.bmiHeader.biBitCount = 24;
	i.bmiHeader.biSizeImage = 0;
	i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	i.bmiHeader.biClrUsed = 0;
	i.bmiHeader.biClrImportant = 0;
	VOID *pvBits;
	HBITMAP bitmap = CreateDIBSection(fakeDC, &i, DIB_RGB_COLORS, &pvBits, NULL, 0);

	SelectObject(fakeDC, this->font);
	SIZE textSize;
	GetTextExtentPoint32(fakeDC, text.c_str(), wcslen(text.c_str()), &textSize);

	return textSize.cx;

}
HWND ExpandButtonWindow::GetHandle()
{
	return this->hWindow;
}

HINSTANCE ExpandButtonWindow::GetInstance()
{
	return this->hInstance;
}

void ExpandButtonWindow::Destroy()
{
	DestroyWindow(this->hWindow);
}


void ExpandButtonWindow::InitializeFonts()
{
	HDC hdc = GetDC(hWindow);
	long lfHeightSmall = -MulDiv(int(20 * 3 / 7.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
	this->font = CreateFont(lfHeightSmall, 0, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
}


LRESULT CALLBACK ExpandButtonWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = hWnd;
	tme.dwHoverTime = 10;

	ExpandButtonWindow* instance = (ExpandButtonWindow*)dwRefData;

	switch (message)
	{
	case WM_MOUSEMOVE:		
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		TrackMouseEvent(&tme);
		
		return TRUE;

	case WM_MOUSEHOVER:
		SetWindowLong(hWnd, GWL_USERDATA, 1);
		InvalidateRect(hWnd, NULL, TRUE);
		
		SetCursor(LoadCursor(NULL, IDC_HAND));
		
		break;

	case WM_SETCURSOR:
		return TRUE;

	case WM_MOUSELEAVE:
		SetWindowLong(hWnd, GWL_USERDATA, 0);
		InvalidateRect(hWnd, NULL, TRUE);
		
		SetCursor(LoadCursor(NULL, IDC_ARROW));	
		
		return TRUE;

	default:
		break;
	}
	return DefSubclassProc(hWnd, message, wParam, lParam);
}

ExpandButtonWindow::~ExpandButtonWindow()
{
	DeleteObject(this->font);
}