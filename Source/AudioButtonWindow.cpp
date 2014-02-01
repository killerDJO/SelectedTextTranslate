#include "stdafx.h"
#include "AudioButtonWindow.h"
#include "TranslateResult.h"

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

	HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AUDIO_INACTIVE));
	SendMessage(this->hWindow, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}

HWND AudioButtonWindow::GetHandle()
{
	return this->hWindow;
}

HINSTANCE AudioButtonWindow::GetInstance()
{
	return this->hInstance;
}

LRESULT CALLBACK AudioButtonWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = hWnd;
	tme.dwHoverTime = HOVER_DEFAULT;

	AudioButtonWindow* instance = (AudioButtonWindow*)dwRefData;
	HBITMAP hBitmap;
	switch (message)
	{
	case WM_MOUSEMOVE:
		hBitmap = LoadBitmap(instance->GetInstance(), MAKEINTRESOURCE(IDB_AUDIO));
		SendMessage(hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		TrackMouseEvent(&tme);
		return TRUE;
	case WM_MOUSELEAVE:
		hBitmap = LoadBitmap(instance->GetInstance(), MAKEINTRESOURCE(IDB_AUDIO_INACTIVE));
		SendMessage(hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		return TRUE;

	default:
		break;
	}
	return DefSubclassProc(hWnd, message, wParam, lParam);
}