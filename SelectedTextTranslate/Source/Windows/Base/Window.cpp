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
    this->position = this->descriptor.Position;
    this->contentSize = Size(0, 0);

    this->activeChildWindows = vector<Window*>();
    this->destroyBeforeDrawList = vector<Window*>();

    this->windowHandle = nullptr;
    this->className = nullptr;
    this->deviceContextBuffer = new DeviceContextBuffer(context->GetDeviceContextProvider(), this->descriptor.WindowSize);

    isVisible = true;

    windowState = WindowStates::New;
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

    windowState = WindowStates::Initialized;
}

void Window::Render(bool preserveVerticalScroll)
{
    int verticalScroll = 0;
    bool rememberScrollPosition = preserveVerticalScroll && descriptor.OverflowY == OverflowModes::Scroll;

    if (rememberScrollPosition)
    {
        verticalScroll = context->GetScrollProvider()->GetScrollPosition(this, ScrollBars::Vertical);
    }

    contentSize = RenderToBuffer();
    windowSize = descriptor.WindowSize;

    if (descriptor.OverflowX == OverflowModes::Stretch && contentSize.Width > descriptor.WindowSize.Width)
    {
        windowSize.Width = contentSize.Width;
    }

    if (descriptor.OverflowY == OverflowModes::Stretch && contentSize.Height > descriptor.WindowSize.Height)
    {
        windowSize.Height = contentSize.Height;
    }

    HDC deviceContext = GetDC(windowHandle);
    Draw(deviceContext, true);
    DeleteDC(deviceContext);

    MoveWindow(windowHandle, descriptor.Position.X, descriptor.Position.Y, windowSize.Width, windowSize.Height, TRUE);

    context->GetScrollProvider()->InitializeScrollbars(
        this,
        descriptor.OverflowX == OverflowModes::Scroll,
        descriptor.OverflowY == OverflowModes::Scroll);

    if (rememberScrollPosition)
    {
        context->GetScrollProvider()->SetScrollPosition(this, ScrollBars::Vertical, verticalScroll);
    }

    windowState = WindowStates::Rendered;
}

Size Window::RenderToBuffer()
{
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    Size renderedSize = RenderContent(renderer);

    Size deviceContextBufferSize = deviceContextBuffer->GetSize();
    int requiredDcWidth = descriptor.OverflowX != OverflowModes::Fixed && renderedSize.Width > deviceContextBufferSize.Width
        ? renderedSize.Width
        : deviceContextBufferSize.Width;
    int requiredDcHeight = descriptor.OverflowY != OverflowModes::Fixed && renderedSize.Height > deviceContextBufferSize.Height
        ? renderedSize.Height
        : deviceContextBufferSize.Height;

    Size requiredDcSize(requiredDcWidth, requiredDcHeight);

    if (!requiredDcSize.Equals(deviceContextBufferSize))
    {
        deviceContextBuffer->Resize(requiredDcSize);
    }

    renderer->Render(deviceContextBuffer);

    context->GetRenderingContext()->ReleaseRenderer(renderer);

    return renderedSize;
}

void Window::Draw(HDC deviceContext, bool drawChildren)
{
    if(drawChildren)
    {
        // Important to draw child windows before drawing parent window.
        // Otherwise, WM_CLIPCHILDREN style will cause redraw of the parent window after the child windows draw.
        DestroyChildWindows(destroyBeforeDrawList);
        DrawChildWindows();
    }

    deviceContextBuffer->Render(deviceContext, windowSize);

    windowState = WindowStates::Drawn;
}

void Window::Redraw()
{
    HDC deviceContext = GetDC(windowHandle);
    Draw(deviceContext, false);
    DeleteDC(deviceContext);

    windowState = WindowStates::Drawn;
}

void Window::DrawChildWindows()
{
    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        Window* childWindow = activeChildWindows[i];
        if(childWindow->IsVisible())
        {
            ShowWindow(childWindow->GetHandle(), SW_SHOW);
            childWindow->Redraw();
            childWindow->DrawChildWindows();
        }
        else
        {
            ShowWindow(childWindow->GetHandle(), SW_HIDE);
        }
    }
}

void Window::AddChildWindow(Window* childWindow)
{
    childWindow->Initialize();
    activeChildWindows.push_back(childWindow);
}

void Window::DestroyChildWindows()
{
    destroyBeforeDrawList.insert(destroyBeforeDrawList.end(), activeChildWindows.begin(), activeChildWindows.end());
    activeChildWindows = vector<Window*>();
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

void Window::Resize()
{
    // By default, do nothing on resize.
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
    return windowHandle;
}

Size Window::GetSize() const
{
    return windowSize;
}

Size Window::GetAvailableClientSize() const
{
    RECT clientRect;
    GetClientRect(windowHandle, &clientRect);

    return Size(
        clientRect.right + GetScrollBarSize(ScrollBars::Vertical),
        clientRect.bottom + GetScrollBarSize(ScrollBars::Horizontal));
}

Size Window::GetCurrentClientSize() const
{
    RECT clientRect;
    GetClientRect(windowHandle, &clientRect);

    return Size(clientRect.right, clientRect.bottom);
}

int Window::GetScrollBarSize(ScrollBars scrollBar) const
{
    SCROLLBARINFO scrollBarInfo;
    scrollBarInfo.cbSize = sizeof SCROLLBARINFO;

    GetScrollBarInfo(
        windowHandle,
        scrollBar == ScrollBars::Vertical ? OBJID_VSCROLL : OBJID_HSCROLL,
        &scrollBarInfo);

    if ((scrollBarInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE) != STATE_SYSTEM_INVISIBLE)
    {
        int scrollBarSize = scrollBar == ScrollBars::Vertical
            ? context->GetScrollProvider()->GetVerticalScrollBarWidth()
            : context->GetScrollProvider()->GetHorizontalScrollBarHeight();
        return scrollBarSize;
    }

    return 0;
}

Size Window::GetContentSize() const
{
    return contentSize;
}

Point Window::GetPosition() const
{
    return position;
}

void Window::Show()
{
    isVisible = true;
}

void Window::Hide()
{
    isVisible = false;
}

bool Window::IsVisible() const
{
    return isVisible;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* instance = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {

    case WM_CREATE:
    {
        // Store pointer to the current window class in the GWLP_USERDATA. 'this' must be passed as lpParam in CreateWindow call.
        CREATESTRUCT* createstruct = (CREATESTRUCT*)lParam;
        instance = (Window*)createstruct->lpCreateParams;
        instance->windowHandle = hWnd;
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
    {
        PAINTSTRUCT ps;
        HDC deviceContext = BeginPaint(instance->GetHandle(), &ps);
        instance->Draw(deviceContext, true);
        EndPaint(instance->GetHandle(), &ps);
        break;
    }

    // Prevent of the background erase reduces flickering before Draw.
    case WM_ERASEBKGND:
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

Window::~Window()
{
    delete deviceContextBuffer;
    DestroyChildWindows(activeChildWindows);
    DestroyChildWindows(destroyBeforeDrawList);
    DestroyWindow(windowHandle);

    windowState = WindowStates::Destroyed;
}