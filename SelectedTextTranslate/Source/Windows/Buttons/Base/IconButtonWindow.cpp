#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Buttons\Base\IconButtonWindow.h"

IconButtonWindow::IconButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
{
	this->hInstance = hInstance;
	this->hWindow = CreateWindowA(
		"STATIC",
		NULL,
		WS_CHILD | WS_VISIBLE | SS_REALSIZECONTROL | SS_BITMAP | SS_NOTIFY,
		x,
		y,
		width,
		height,
		parentWindow,
		NULL,
		this->hInstance,
		NULL);

	SetWindowSubclass(this->hWindow, IconButtonWindow::WndProc, 0, (DWORD)this);
}

HWND IconButtonWindow::GetHandle()
{
	return this->hWindow;
}

HINSTANCE IconButtonWindow::GetInstance()
{
	return this->hInstance;
}

HBITMAP IconButtonWindow::GetNormalImage()
{
	return this->imageNormal;
}

HBITMAP IconButtonWindow::GetHoverImage()
{
	return this->imageHover;
}

void IconButtonWindow::Show()
{
	BringWindowToTop(this->hWindow);
	ShowWindow(this->hWindow, SW_SHOW);
}

void IconButtonWindow::Hide()
{
	ShowWindow(this->hWindow, SW_HIDE);
}

LRESULT CALLBACK IconButtonWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = hWnd;
	tme.dwHoverTime = 10;

	IconButtonWindow* instance = (IconButtonWindow*)dwRefData;
	switch (message)
	{
	case WM_MOUSEMOVE:
		if ((GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCH) != MOUSEEVENTF_FROMTOUCH) {
			SendMessage(hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)instance->GetHoverImage());
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			TrackMouseEvent(&tme);
		}
		return TRUE;

	case WM_MOUSEHOVER:
		SetCursor(LoadCursor(NULL, IDC_HAND));
		break;

	case WM_SETCURSOR:
		return TRUE;

	case WM_MOUSELEAVE:
		SendMessage(hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)instance->GetNormalImage());

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return TRUE;

	default:
		break;
	}
	return DefSubclassProc(hWnd, message, wParam, lParam);
}


IconButtonWindow::~IconButtonWindow()
{
	DeleteObject(this->imageHover);
	DeleteObject(this->imageNormal);
}
