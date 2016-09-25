#include "Windows\Base\Window.h"

Window::Window(WindowContext* context, WindowDescriptor descriptor)
{
    if(descriptor.AutoScale)
    {
        this->descriptor = context->GetScaleProvider()->Scale(descriptor);
    }
    else
    {
        this->descriptor = descriptor;
    }

    this->context = context;

    this->windowSize = this->descriptor.WindowSize;
    this->dcSize = this->descriptor.WindowSize;
    this->position = this->descriptor.Position;
    this->contentSize = Size(0, 0);

    this->activeChildWindows = vector<Window*>();
    this->destroyBeforeDrawQueue = vector<Window*>();

    this->hWindow = nullptr;
    this->className = nullptr;
    this->inMemoryDC = context->GetDeviceContextProvider()->CreateInMemoryDC(dcSize);
}

void Window::Initialize()
{
    WNDCLASSEX windowClass = { 0 };

    if (!GetClassInfoEx(context->GetInstance(), className, &windowClass))
    {
        windowClass.hInstance = context->GetInstance();
        windowClass.lpszClassName = className;
        windowClass.lpfnWndProc = WndProc;
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        SpecifyWindowClass(&windowClass);

        if (!RegisterClassEx(&windowClass))
        {
            FatalAppExit(0, TEXT("Couldn't register window class!"));
        }
    }
}

void Window::Render(bool preserveVerticalScroll)
{
    int verticalScroll = 0;

    if (preserveVerticalScroll && descriptor.OverflowY == OverflowModes::Scroll)
    {
        verticalScroll = context->GetScrollProvider()->GetScrollPosition(this, ScrollBars::Vertical);
    }

    Size renderedSize = RenderContent();
   
    windowSize = descriptor.WindowSize;
    contentSize = renderedSize;

    if (descriptor.OverflowX == OverflowModes::Stretch && contentSize.Width > descriptor.WindowSize.Width)
    {
        windowSize.Width = contentSize.Width;
    }

    if (descriptor.OverflowY == OverflowModes::Stretch && contentSize.Height > descriptor.WindowSize.Height)
    {
        windowSize.Height = contentSize.Height;
    }

    if (descriptor.OverflowX == OverflowModes::Scroll)
    {
        context->GetScrollProvider()->InitializeScrollbar(this, contentSize.Width, ScrollBars::Horizontal);
    }

    if (descriptor.OverflowY == OverflowModes::Scroll)
    {
        context->GetScrollProvider()->InitializeScrollbar(this, contentSize.Height, ScrollBars::Vertical);

        if (preserveVerticalScroll)
        {
            context->GetScrollProvider()->SetScrollPosition(this, ScrollBars::Vertical, verticalScroll);
        }
    }

    InvalidateRect(hWindow, nullptr, FALSE);
    MoveWindow(hWindow, descriptor.Position.X, descriptor.Position.Y, windowSize.Width, windowSize.Height, FALSE);
}

Size Window::RenderContent()
{
    destroyBeforeDrawQueue.insert(destroyBeforeDrawQueue.end(), activeChildWindows.begin(), activeChildWindows.end());
    activeChildWindows = vector<Window*>();

    Renderer* renderer = context->GetRenderingContext()->GetRenderer(inMemoryDC);
    
    Size renderedSize = RenderDC(renderer);
    
    context->GetRenderingContext()->ReleaseRenderer(renderer);
    
    int requiredDcWidth = descriptor.OverflowX != OverflowModes::Fixed && renderedSize.Width > dcSize.Width
        ? renderedSize.Width
        : dcSize.Width;
    int requiredDcHeight = descriptor.OverflowY != OverflowModes::Fixed && renderedSize.Height > dcSize.Height
        ? renderedSize.Height
        : dcSize.Height;

    Size requiredDcSize(requiredDcWidth, requiredDcHeight);

    // Resize buffering DC if neccesssary
    if (!requiredDcSize.Equals(dcSize))
    {
        dcSize = requiredDcSize;
        context->GetDeviceContextProvider()->ResizeDC(inMemoryDC, dcSize);

        renderer = context->GetRenderingContext()->GetRenderer(inMemoryDC);
        RenderDC(renderer);
        context->GetRenderingContext()->ReleaseRenderer(renderer);
    }

    return renderedSize;
}

void Window::Draw()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWindow, &ps);

    context->GetDeviceContextProvider()->CopyDC(inMemoryDC, hdc, windowSize);

    EndPaint(hWindow, &ps);

    DestroyChildWindows(destroyBeforeDrawQueue);
    DrawChildWindows();
}

void Window::ForceDraw() const
{
    HDC hdc = GetDC(hWindow);
    context->GetDeviceContextProvider()->CopyDC(inMemoryDC, hdc, windowSize);
    DeleteDC(hdc);
}

void Window::DrawChildWindows()
{
    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        Window* childWindow = activeChildWindows[i];
        childWindow->ForceDraw();
        childWindow->DrawChildWindows();
        childWindow->Show();
    }
}

void Window::AddChildWindow(Window* childWindow)
{
    childWindow->Initialize();
    activeChildWindows.push_back(childWindow);
}

void Window::DestroyChildWindows(vector<Window*>& childWindows) const
{
    for (size_t i = 0; i < childWindows.size(); ++i)
    {
        delete childWindows[i];
    }

    childWindows.clear();
    childWindows.resize(0);
}

DWORD Window::GetScrollStyle() const
{
    int scrollStyle = 0;

    if (descriptor.OverflowX == OverflowModes::Scroll)
    {
        scrollStyle |= WS_HSCROLL;
    }

    if (descriptor.OverflowY == OverflowModes::Scroll)
    {
        scrollStyle |= WS_VSCROLL;
    }

    return scrollStyle;
}

HWND Window::GetHandle() const
{
    return hWindow;
}

int Window::GetWidth() const
{
    return windowSize.Width;
}

int Window::GetHeight() const
{
    return windowSize.Height;
}

Point Window::GetPosition() const
{
    return position;
}

void Window::Show() const
{
    ShowWindow(hWindow, SW_SHOW);
}

void Window::Hide() const
{
    ShowWindow(hWindow, SW_HIDE);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* instance = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {

    case WM_CREATE:
    {
        CREATESTRUCT* createstruct = (CREATESTRUCT*)lParam;
        instance = (Window*)createstruct->lpCreateParams;
        instance->hWindow = hWnd;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createstruct->lpCreateParams);
        break;
    }

    case WM_HSCROLL:
        if(instance->descriptor.OverflowX == OverflowModes::Scroll)
        {
            instance->context->GetScrollProvider()->ProcessHorizontalScroll(instance, wParam, lParam);
        }
        break;

    case WM_VSCROLL:
        if (instance->descriptor.OverflowY == OverflowModes::Scroll)
        {
            instance->context->GetScrollProvider()->ProcessVerticalScroll(instance, wParam, lParam);
        }
        break;

    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta < 0)
        {
            SendMessage(instance->GetHandle(), WM_VSCROLL, SB_LINEDOWN, NULL);
        }
        else
        {
            SendMessage(instance->GetHandle(), WM_VSCROLL, SB_LINEUP, NULL);
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    case WM_PAINT:
        instance->Draw();
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

Window::~Window()
{
    DestroyWindow(hWindow);
    DeleteDC(inMemoryDC);

    DestroyChildWindows(activeChildWindows);
    DestroyChildWindows(destroyBeforeDrawQueue);
}