#include "Windows\Base\ChildWindow.h"

ChildWindow::ChildWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
    : Window(hInstance)
{
    this->parentWindow = parentWindow;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->className = L"STT_CONTENT";

    this->activeChildWindows = vector<ChildWindow*>();
    this->childWindowsToDestory = vector<ChildWindow*>();
}

void ChildWindow::Initialize()
{
    Window::Initialize();

    InitializeInMemoryDC();

    hWindow = CreateWindow(
        className,
        NULL,
        WS_CHILD | WS_CLIPCHILDREN,
        x,
        y,
        width,
        height,
        parentWindow,
        NULL,
        hInstance,
        this);

    ComputeParameters();
    InitializeFonts();
    InitializeBrushes();
}

void ChildWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->lpfnWndProc = WndProc;
    windowClass->hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void ChildWindow::InitializeInMemoryDC()
{
    inMemoryDC = CreateInMemoryDC(width, height);
    ClearDC(inMemoryDC);
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

HDC ChildWindow::CreateInMemoryDC(DWORD hdcWidth, DWORD hdcHeight)
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

void ChildWindow::ResizeDC(HDC &hdc, DWORD width, DWORD height)
{
    DeleteDC(hdc);
    hdc = CreateInMemoryDC(width, height);
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

POINT ChildWindow::RenderDC()
{
    childWindowsToDestory = activeChildWindows;
    activeChildWindows = vector<ChildWindow*>();

    ClearDC(inMemoryDC);

    return POINT();
}

void ChildWindow::ClearDC(HDC hdc)
{
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = height;
    rect.right = width;
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

DWORD ChildWindow::CopyDC(HDC source, HDC target)
{
    return BitBlt(target, 0, 0, width, height, source, 0, 0, SRCCOPY);
}

void ChildWindow::Draw()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWindow, &ps);

    DWORD res = CopyDC(inMemoryDC, hdc);

    EndPaint(hWindow, &ps);

    DestroyChildWindows(&childWindowsToDestory);
    RedrawChildWindows();
}

void ChildWindow::RedrawChildWindows()
{
    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        ChildWindow* childWindow = activeChildWindows[i];
        childWindow->ForceRedraw();
        childWindow->RedrawChildWindows();
    }
}

void ChildWindow::ForceRedraw()
{
    HDC hdc = GetDC(hWindow);
    DWORD res = CopyDC(inMemoryDC, hdc);
    DeleteDC(hdc);
}

DWORD ChildWindow::AdjustToResolution(double value, double k)
{
    return roundToInt(value * k);
}

void ChildWindow::AddChildWindow(ChildWindow* childWindow)
{
    childWindow->Initialize();
    childWindow->Show();
    activeChildWindows.push_back(childWindow);
}

void ChildWindow::DestroyChildWindows(vector<ChildWindow*>* childWindows)
{
    for (size_t i = 0; i < childWindows->size(); ++i)
    {
        delete (*childWindows)[i];
    }

    childWindows->clear();
    childWindows->resize(0);
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
    DestroyWindow(hWindow);
    DeleteDC(inMemoryDC);

    DestroyChildWindows(&activeChildWindows);
    DestroyChildWindows(&childWindowsToDestory);
}