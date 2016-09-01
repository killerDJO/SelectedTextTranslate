#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Helpers\Utilities.h"

class WindowBase
{
private:	
	void InitializeInMemoryDC();
	void DestroyChildWindows();

protected:
	DWORD initialX, initialY;
	DWORD width, height;

	HWND parentWindow;
	HWND hWindow;
	HINSTANCE hInstance;
	HDC inMemoryHDC;
	vector<WindowBase*> childWindows;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual void ComputeParameters();
	virtual void InitializeFonts();
	virtual void InitializeBrushes();
	
	virtual POINT RenderDC();
	void ResetWindow(POINT bottomRight);
	void Draw();

	DWORD AdjustToResolution(double value, double k);
	HDC CreateInMemoryHDC(DWORD hdcWidth, DWORD hdcHeight);
	void ClearHDC(HDC hdc);
	void ResizeHDC(HDC &hdc, DWORD width, DWORD height);

public:
	WindowBase(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~WindowBase();

	HWND GetHandle();
	HINSTANCE GetInstance();
	DWORD GetWidth();
	DWORD GetHeight();

	virtual void Initialize();
	void Show();
	void Hide();
	void Destroy();
};