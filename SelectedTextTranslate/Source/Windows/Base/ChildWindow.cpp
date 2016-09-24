#include "Windows\Base\ChildWindow.h"

ChildWindow::ChildWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow)
    : Window(hInstance, renderingContext, scrollProvider, descriptor), inMemoryDC(nullptr)
{
    this->parentWindow = parentWindow;

    this->className = L"STT_CONTENT";

    this->activeChildWindows = vector<ChildWindow*>();
    this->destroyBeforeDrawQueue = vector<ChildWindow*>();

    this->dcWidth = descriptor.Width;
    this->dcHeight = descriptor.Height;
}

void ChildWindow::Initialize()
{
    Window::Initialize();

    inMemoryDC = renderingContext->CreateInMemoryDC(dcWidth, dcHeight);

    hWindow = CreateWindow(
        className,
        NULL,
        WS_CHILD | WS_CLIPCHILDREN,
        descriptor.X,
        descriptor.Y,
        descriptor.Width,
        descriptor.Height,
        parentWindow,
        NULL,
        hInstance,
        this);
}

void ChildWindow::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    windowClass->lpfnWndProc = WndProc;
    windowClass->hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
        InvalidateRect(hWnd, nullptr, FALSE);
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
        return Window::WndProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

SIZE ChildWindow::RenderContent()
{
    destroyBeforeDrawQueue.insert(destroyBeforeDrawQueue.end(), activeChildWindows.begin(), activeChildWindows.end());
    activeChildWindows = vector<ChildWindow*>();

    Renderer* renderer = new Renderer(inMemoryDC, renderingContext);
    SIZE renderedSize = RenderDC(renderer);
    delete renderer;

    int requiredDcWidth = descriptor.OverflowX != OverflowModes::Fixed && renderedSize.cx > dcWidth ? renderedSize.cx : dcWidth;
    int requiredDcHeight = descriptor.OverflowY != OverflowModes::Fixed && renderedSize.cy > dcHeight ? renderedSize.cy : dcHeight;

    if (requiredDcWidth != dcWidth || requiredDcHeight != dcHeight)
    {
        dcWidth = requiredDcWidth;
        dcHeight = requiredDcHeight;
        renderingContext->ResizeDC(inMemoryDC, dcWidth, dcHeight);

        renderer = new Renderer(inMemoryDC, renderingContext);
        RenderDC(renderer);
        delete renderer;
    }

    return renderedSize;
}

void ChildWindow::Draw()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWindow, &ps);

    renderingContext->CopyDC(inMemoryDC, hdc, currentWidth, currentHeight);

    EndPaint(hWindow, &ps);

    DestroyChildWindows(destroyBeforeDrawQueue);
    DrawChildWindows();
}

void ChildWindow::ForceDraw() const
{
    HDC hdc = GetDC(hWindow);
    renderingContext->CopyDC(inMemoryDC, hdc, currentWidth, currentHeight);
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

void ChildWindow::DestroyChildWindows(vector<ChildWindow*>& childWindows) const
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
    DeleteDC(inMemoryDC);

    DestroyChildWindows(activeChildWindows);
    DestroyChildWindows(destroyBeforeDrawQueue);
}