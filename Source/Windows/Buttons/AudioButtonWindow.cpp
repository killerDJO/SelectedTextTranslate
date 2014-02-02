#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Buttons\AudioButtonWindow.h"

AudioButtonWindow::AudioButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y)
{
	this->hInstance = hInstance;
	this->hWindow = CreateWindowA(
		"STATIC",
		NULL,
		WS_CHILD | WS_VISIBLE | SS_REALSIZECONTROL | SS_BITMAP | SS_NOTIFY,
		x,
		y,
		15,
		15,
		parentWindow,
		NULL,
		this->hInstance,
		NULL);

	SetWindowSubclass(this->hWindow, AudioButtonWindow::WndProc, 0, (DWORD)this);

	this->InitializeBitmaps();
	
	SendMessage(this->hWindow, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)this->imageNormal);
}

void AudioButtonWindow::InitializeBitmaps()
{
	this->imageNormal = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AUDIO_INACTIVE));
	this->imageHover = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AUDIO));
}

HWND AudioButtonWindow::GetHandle()
{
	return this->hWindow;
}

HINSTANCE AudioButtonWindow::GetInstance()
{
	return this->hInstance;
}

HBITMAP AudioButtonWindow::GetNormalImage()
{
	return this->imageNormal;
}

HBITMAP AudioButtonWindow::GetHoverImage()
{
	return this->imageHover;
}

void AudioButtonWindow::Show()
{
	ShowWindow(this->hWindow, SW_SHOW);
}

void AudioButtonWindow::Hide()
{
	ShowWindow(this->hWindow, SW_HIDE);
}

LRESULT CALLBACK AudioButtonWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = hWnd;
	tme.dwHoverTime = 10;

	AudioButtonWindow* instance = (AudioButtonWindow*)dwRefData;
	HBITMAP hBitmap;
	switch (message)
	{
	case WM_MOUSEMOVE:
		SendMessage(hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)instance->GetHoverImage());

		tme.dwFlags = TME_HOVER | TME_LEAVE;
		TrackMouseEvent(&tme);
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

AudioButtonWindow::~AudioButtonWindow()
{
	DeleteObject(this->imageHover);
	DeleteObject(this->imageNormal);
}