#include "Windows\Base\ChildWindow.h"

ChildWindow::ChildWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
{
	this->parentWindow = parentWindow;
	this->hInstance = hInstance;
	this->initialX = x;
	this->initialY = y;
	this->width = width;
	this->height = height;	
}

void ChildWindow::Initialize()
{
	const TCHAR* className = TEXT("STT_CONTENT");
	WNDCLASSEX wnd = { 0 };

	if (!GetClassInfoEx(hInstance, className, &wnd))
	{
		wnd.style = RDW_ALLCHILDREN;
		wnd.hInstance = hInstance;
		wnd.lpszClassName = className;
		wnd.lpfnWndProc = WndProc;
		wnd.cbSize = sizeof(WNDCLASSEX);
		wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

		if (!RegisterClassEx(&wnd))
		{
			FatalAppExit(0, TEXT("Couldn't register window class!"));
		}
	}

	this->InitializeInMemoryDC();

	this->hWindow = CreateWindow(
		className,
		NULL,
		WS_CHILD | WS_VISIBLE,
		this->initialX,
		this->initialY,
		this->width,
		this->height,
		this->parentWindow,
		NULL,
		this->hInstance,
		this);

	this->ComputeParameters();
	this->InitializeFonts();
	this->InitializeBrushes();
}

void ChildWindow::InitializeInMemoryDC()
{
	this->inMemoryHDC = CreateInMemoryHDC(this->width, this->height);
	ClearHDC(this->inMemoryHDC);
}

void ChildWindow::ComputeParameters()
{
}

void ChildWindow::InitializeFonts()
{
}

void ChildWindow::InitializeBrushes()
{
}

HDC ChildWindow::CreateInMemoryHDC(DWORD hdcWidth, DWORD hdcHeight)
{
	HDC hdc = CreateCompatibleDC(NULL);

	BITMAPINFO i;
	ZeroMemory(&i.bmiHeader, sizeof(BITMAPINFOHEADER));
	i.bmiHeader.biWidth = hdcWidth;
	i.bmiHeader.biHeight = hdcHeight;
	i.bmiHeader.biPlanes = 1;
	i.bmiHeader.biBitCount = 24;
	i.bmiHeader.biSizeImage = 0;
	i.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	i.bmiHeader.biClrUsed = 0;
	i.bmiHeader.biClrImportant = 0;
	VOID *pvBits;
	HBITMAP bitmap = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, &pvBits, NULL, 0);

	SelectObject(hdc, bitmap);

	return hdc;
}

void ChildWindow::ResizeHDC(HDC &hdc, DWORD width, DWORD height)
{
	DeleteDC(hdc);
	hdc = CreateInMemoryHDC(width, height);
}

HWND ChildWindow::GetHandle()
{
	return this->hWindow;
}

HINSTANCE ChildWindow::GetInstance()
{
	return this->hInstance;
}

DWORD ChildWindow::GetWidth()
{
	return this->width;
}

DWORD ChildWindow::GetHeight()
{
	return this->height;
}

LRESULT CALLBACK ChildWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ChildWindow* instance = (ChildWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{

	case WM_NCCREATE:
	case WM_CREATE:
	{
		CREATESTRUCT* createstruct = (CREATESTRUCT*)lParam;
		instance = (ChildWindow*)createstruct->lpCreateParams;
		instance->hWindow = hWnd;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createstruct->lpCreateParams);

		return TRUE;
	}

	case WM_MOVE:
	{
		RECT rcWindow;
		POINTS pos = MAKEPOINTS(lParam);
		GetWindowRect(hWnd, &rcWindow);
		MoveWindow(hWnd, pos.x, pos.y, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, FALSE);
		InvalidateRect(hWnd, NULL, FALSE);
		return TRUE;
	}

	case WM_NCPAINT:
	case WM_PAINT:
	case WM_SHOWWINDOW:
		instance->Draw();
		break;

	case WM_HSCROLL:
	case WM_VSCROLL:
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void ChildWindow::ResetWindow(POINT bottomRight)
{
	MoveWindow(this->hWindow, this->initialX, this->initialY, bottomRight.x, bottomRight.y, FALSE);
}

POINT ChildWindow::RenderDC()
{
	DestroyChildWindows();

	ClearHDC(this->inMemoryHDC);

	return POINT();
}

void ChildWindow::ClearHDC(HDC hdc)
{
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.bottom = height;
	rect.right = width;
	FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
}

void ChildWindow::Draw()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(this->hWindow, &ps);

	DWORD res = BitBlt(hdc, 0, 0, width, height, this->inMemoryHDC, 0, 0, SRCCOPY);

	EndPaint(this->hWindow, &ps);

	for (size_t i = 0; i < childWindows.size(); ++i) 
	{
		childWindows[i]->Show();
	}
}

void ChildWindow::Show()
{
	ShowWindow(this->hWindow, SW_SHOW);
}

void ChildWindow::Hide()
{
	ShowWindow(this->hWindow, SW_HIDE);
}

DWORD ChildWindow::AdjustToResolution(double value, double k)
{
	return int(round(value * k));
}

void ChildWindow::DestroyChildWindows()
{
	for (size_t i = 0; i < childWindows.size(); ++i)
	{
		delete childWindows[i];
	}

	childWindows.clear();
	childWindows.resize(0);
}

SIZE ChildWindow::GetTextSize(HDC hdc, const wchar_t* text, HFONT font)
{
	SelectObject(hdc, font);

	SIZE textSize;
	GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

	return textSize;
}

POINT ChildWindow::PrintText(HDC hdc, const wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight)
{
	SelectObject(hdc, font);
	SetTextColor(hdc, color);

	SIZE textSize;
	GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

	TextOut(hdc, x, y, text, _tcslen(text));

	bottomRight->x = max(bottomRight->x, x + textSize.cx);
	bottomRight->y = max(bottomRight->y, y + textSize.cy);

	POINT result;
	result.x = x + textSize.cx;
	result.y = y + textSize.cy;
	return result;
}


void ChildWindow::DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight)
{
	FillRect(hdc, &rect, brush);

	bottomRight->x = max(bottomRight->x, rect.right);
	bottomRight->y = max(bottomRight->y, rect.bottom);
}

ChildWindow::~ChildWindow()
{
	DestroyWindow(this->hWindow);
	DeleteDC(this->inMemoryHDC);
	DestroyChildWindows();
}