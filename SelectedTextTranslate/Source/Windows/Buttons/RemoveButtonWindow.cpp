#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Buttons\RemoveButtonWindow.h"

RemoveButtonWindow::RemoveButtonWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
	: IconButtonWindow(parentWindow, hInstance, x, y, width, height)
{
	this->InitializeBitmaps();

	SendMessage(this->hWindow, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)this->imageNormal);
}

void RemoveButtonWindow::InitializeBitmaps()
{
	this->imageNormal = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AUDIO_INACTIVE));
	this->imageHover = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AUDIO));
}

RemoveButtonWindow::~RemoveButtonWindow()
{
}
