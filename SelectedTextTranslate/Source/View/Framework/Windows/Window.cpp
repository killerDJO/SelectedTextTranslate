#include "View\Framework\Windows\Window.h"
#include "Exceptions\SelectedTextTranslateFatalException.h"
#include "Helpers\StringUtilities.h"
#include "Helpers\ExceptionHelper.h"

Window::Window(WindowContext* context, WindowDescriptor descriptor)
{
    if (descriptor.AutoScale)
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

    this->isVisible = true;
    this->windowState = WindowStates::New;
}

void Window::Initialize()
{
    WNDCLASSEX windowClass = { 0 };

    if(!GetClassInfoEx(context->GetInstance(), className, &windowClass))
    {
        windowClass.hInstance = context->GetInstance();
        windowClass.lpszClassName = className;
        windowClass.lpfnWndProc = WindowProcedureWrapper;
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        SpecifyWindowClass(&windowClass);

        AssertCriticalWinApiResult(RegisterClassEx(&windowClass));
    }

    windowState = WindowStates::Initialized;
}

void Window::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
}

void Window::Render(bool preserveScrolls)
{
    RenderingContext* renderingContext = context->GetRenderingContext();
    renderingContext->BeginRender(this);

    windowState = WindowStates::Rendering;

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

    windowState = WindowStates::Rendered;

    if (renderingContext->IsRenderingRoot(this))
    {
        ApplyRenderedState(preserveScrolls);
    }

    renderingContext->EndRender(this);
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

void Window::ApplyRenderedState(bool preserveScrolls)
{
    // Child windows should be destroyed first
    DestroyChildWindows(destroyBeforeDrawList);

    ApplyWindowPosition(preserveScrolls);

    // Important to draw child windows first
    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        Window* childWindow = activeChildWindows[i];
        if (childWindow->IsVisible())
        {
            ShowWindow(childWindow->GetHandle(), SW_SHOW);
            childWindow->ApplyRenderedState(preserveScrolls);
        }
        else
        {
            ShowWindow(childWindow->GetHandle(), SW_HIDE);
        }
    }

    Draw(false);

    windowState = WindowStates::Drawn;
}

void Window::ApplyWindowPosition(bool preserveScrolls)
{
    int verticalScrollPosition = 0;
    int horizontalScrollPosition = 0;
    if (preserveScrolls)
    {
        verticalScrollPosition = context->GetScrollProvider()->GetCurrentScrollPostion(this, ScrollBars::Vertical);
        horizontalScrollPosition = context->GetScrollProvider()->GetCurrentScrollPostion(this, ScrollBars::Horizontal);
    }

    Point offset = GetInitialWindowOffset();
    AssertCriticalWinApiResult(MoveWindow(windowHandle, descriptor.Position.X - offset.X, descriptor.Position.Y - offset.Y, windowSize.Width, windowSize.Height, FALSE));

    // Important to initialize scroll only after window has been moved
    context->GetScrollProvider()->InitializeScrollbars(
        this,
        descriptor.OverflowX == OverflowModes::Scroll,
        descriptor.OverflowY == OverflowModes::Scroll,
        verticalScrollPosition,
        horizontalScrollPosition);

    // Render scroll immediately
    SendMessage(windowHandle, WM_NCPAINT, NULL, NULL);
}

Point Window::GetInitialWindowOffset()
{
    return Point(0, 0);
}

void Window::Draw(bool drawChildren)
{
    if (drawChildren)
    {
        // Important to draw child windows before drawing parent window.
        // Otherwise, WM_CLIPCHILDREN style will cause redraw of the parent window after the child windows draw.
        DrawChildWindows();
    }

    HDC deviceContext = GetDC(windowHandle);
    AssertCriticalWinApiResult(deviceContext);
    deviceContextBuffer->Render(deviceContext, windowSize);
    AssertCriticalWinApiResult(DeleteDC(deviceContext));
}

void Window::DrawChildWindows()
{
    for (size_t i = 0; i < activeChildWindows.size(); ++i)
    {
        Window* childWindow = activeChildWindows[i];
        childWindow->Draw(true);
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

WindowDescriptor Window::GetDescriptor() const
{
    return this->descriptor;
}

Size Window::GetSize() const
{
    return windowSize;
}

Size Window::GetAvailableClientSize() const
{
    RECT clientRect;
    AssertCriticalWinApiResult(GetClientRect(windowHandle, &clientRect));

    ScrollProvider* scrollProvider = context->GetScrollProvider();
    return Size(
        clientRect.right + scrollProvider->GetScrollBarSize(this, ScrollBars::Vertical),
        clientRect.bottom + scrollProvider->GetScrollBarSize(this, ScrollBars::Horizontal));
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

LRESULT Window::WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* instance;

    if(message == WM_CREATE)
    {
        // Store pointer to the current window class in the GWLP_USERDATA. 'this' must be passed as lpParam in CreateWindow call.
        CREATESTRUCT* createstruct = (CREATESTRUCT*)lParam;
        instance = (Window*)createstruct->lpCreateParams;
        instance->windowHandle = hWnd;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createstruct->lpCreateParams);
    }
    else
    {
        instance = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if(instance == nullptr)
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    try
    {
        return instance->WindowProcedure(message, wParam, lParam);
    }
    catch (const SelectedTextTranslateFatalException& exception)
    {
        instance->TerminateOnException(exception.GetErrorMessage());
    }
    catch (const exception& exception)
    {
        instance->TerminateOnException(StringUtilities::GetUtf16String(exception.what()));
    }
    catch (...)
    {
        instance->TerminateOnException(wstring());
    }

    return -1;
}

LRESULT Window::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    context->GetScrollProvider()->ProcessScrollMessages(this, message, wParam, lParam);

    switch (message)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        AssertCriticalWinApiResult(BeginPaint(GetHandle(), &ps));
        Draw(false);
        EndPaint(GetHandle(), &ps);
        break;
    }

    // Prevent of the background erase reduces flickering before Draw.
    case WM_ERASEBKGND:
        break;

    default:
        return DefWindowProc(windowHandle, message, wParam, lParam);
    }

    return 0;
}

void Window::TerminateOnException(wstring message) const
{
    context->GetLogger()->LogFormatted(L"Unhandled exception occurred. Message: '%ls'.", message.c_str());
    FatalAppExit(0, TEXT("Unhandled exception occurred. See log for details."));
}

Window::~Window()
{
    delete deviceContextBuffer;
    DestroyChildWindows(activeChildWindows);
    DestroyChildWindows(destroyBeforeDrawList);
    AssertCriticalWinApiResult(DestroyWindow(windowHandle));

    windowState = WindowStates::Destroyed;
}