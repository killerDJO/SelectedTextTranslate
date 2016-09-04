#pragma once

class Window
{
protected:
	DWORD x, y;
	DWORD width, height;

	HWND hWindow;
	HINSTANCE hInstance;

	wchar_t* className;

	virtual void SpecifyWindowClass(WNDCLASSEX* windowClass) = 0;

public:
	Window(HINSTANCE hInstance);
	virtual ~Window();

	HWND GetHandle();
	HINSTANCE GetInstance();
	DWORD GetWidth();
	DWORD GetHeight();

	virtual void Initialize();
	void Show();
	void Hide();
};