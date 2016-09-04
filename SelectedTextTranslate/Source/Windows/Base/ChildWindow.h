#pragma once

class ChildWindow
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

	vector<ChildWindow*> childWindows;

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

	POINT PrintText(HDC hdc, const wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight);
	void DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight);
	SIZE GetTextSize(HDC hdc, const wchar_t* text, HFONT font);

public:
	ChildWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	virtual ~ChildWindow();

	HWND GetHandle();
	HINSTANCE GetInstance();
	DWORD GetWidth();
	DWORD GetHeight();

	virtual void Initialize();
	void Show();
	void Hide();
};