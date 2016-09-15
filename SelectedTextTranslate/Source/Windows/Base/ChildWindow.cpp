#include "Windows\Base\ChildWindow.h"

ChildWindow::ChildWindow(Renderer* renderer, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
    : Window(hInstance, renderer)
{
    this->parentWindow = parentWindow;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->className = L"STT_CONTENT";

    this->activeChildWindows = vector<ChildWindow*>();
    this->destroyBeforeDrawQueue = vector<ChildWindow*>();
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
}

void ChildWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->lpfnWndProc = WndProc;
    windowClass->hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void ChildWindow::InitializeInMemoryDC()
{
    inMemoryDC = renderer->CreateInMemoryDC(width, height);
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
    destroyBeforeDrawQueue.insert(destroyBeforeDrawQueue.end(), activeChildWindows.begin(), activeChildWindows.end());
    activeChildWindows = vector<ChildWindow*>();

    return POINT();
}

void ChildWindow::Draw()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWindow, &ps);

    DWORD res = renderer->CopyDC(inMemoryDC, hdc, width, height);

    EndPaint(hWindow, &ps);

    DestroyChildWindows(destroyBeforeDrawQueue);
    DrawChildWindows();
}

void ChildWindow::ForceDraw()
{
    HDC hdc = GetDC(hWindow);
    DWORD res = renderer->CopyDC(inMemoryDC, hdc, width, height);
    DeleteDC(hdc);
}

void ChildWindow::DrawChildWindows()
{
    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        ChildWindow* childWindow = activeChildWindows[i];
        childWindow->ForceDraw();
        childWindow->DrawChildWindows();
    }
}

void ChildWindow::AddChildWindow(ChildWindow* childWindow)
{
    childWindow->Initialize();
    childWindow->Show();
    activeChildWindows.push_back(childWindow);
}

void ChildWindow::DestroyChildWindows(vector<ChildWindow*>& childWindows)
{
    for (size_t i = 0; i < childWindows.size(); ++i)
    {
        delete childWindows[i];
    }

    childWindows.clear();
    childWindows.resize(0);
}

ChildWindow::~ChildWindow()
{
    DestroyWindow(hWindow);
    DeleteDC(inMemoryDC);

    DestroyChildWindows(activeChildWindows);
    DestroyChildWindows(destroyBeforeDrawQueue);
}