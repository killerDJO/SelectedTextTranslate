#include "View\Framework\Windows\Window.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

Window::Window(WindowContext* context)
    : NativeWindowHolder(context->GetInstance())
{
    if(context == nullptr)
    {
        throw SelectedTextTranslateFatalException(L"Window context must be provided.");
    }

    this->context = context;
    this->descriptor = WindowDescriptor();
    this->activeChildWindows = vector<Window*>();
    this->destroyBeforeDrawList = vector<Window*>();

    this->className = nullptr;
    this->deviceContextBuffer = nullptr;

    this->isVisible = true;
    this->windowState = WindowStates::New;
}

WindowDescriptor Window::GetDescriptor() const
{
    return descriptor;
}

void Window::SetDescriptor(WindowDescriptor descriptor)
{
    AssertWindowNotInitialized();
    this->descriptor = descriptor;
}

void Window::Initialize()
{
    if(descriptor.IsEmpty())
    {
        throw SelectedTextTranslateException(L"Descriptor must be set for window");
    }

    if (descriptor.IsAutoScaleEnabled())
    {
        descriptor = context->GetScaleProvider()->Scale(descriptor);
    }

    deviceContextBuffer = new DeviceContextBuffer(context->GetDeviceContextProvider(), descriptor.GetWindowSize());

    windowSize = descriptor.GetWindowSize();
    position = descriptor.GetPosition();
    contentSize = Size(0, 0);

    NativeWindowHolder::Initialize();
    windowState = WindowStates::Initialized;
}

void Window::Render(bool preserveScrolls)
{
    AssertWindowInitialized();

    RenderingContext* renderingContext = context->GetRenderingContext();
    renderingContext->BeginRender(this);

    windowState = WindowStates::Rendering;

    contentSize = RenderToBuffer();
    windowSize = descriptor.GetWindowSize();

    if (descriptor.GetOverflowX() == OverflowModes::Stretch && contentSize.Width > descriptor.GetWindowSize().Width)
    {
        windowSize.Width = contentSize.Width;
    }

    if (descriptor.GetOverflowY() == OverflowModes::Stretch && contentSize.Height > descriptor.GetWindowSize().Height)
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
    int requiredDcWidth = descriptor.GetOverflowX() != OverflowModes::Fixed && renderedSize.Width > deviceContextBufferSize.Width
        ? renderedSize.Width
        : deviceContextBufferSize.Width;
    int requiredDcHeight = descriptor.GetOverflowY() != OverflowModes::Fixed && renderedSize.Height > deviceContextBufferSize.Height
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
            childWindow->Show();
        }
        else
        {
            childWindow->Hide();
        }

        childWindow->ApplyRenderedState(preserveScrolls);
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
    AssertCriticalWinApiResult(MoveWindow(windowHandle, descriptor.GetPosition().X - offset.X, descriptor.GetPosition().Y - offset.Y, windowSize.Width, windowSize.Height, FALSE));

    // Important to initialize scroll only after window has been moved
    context->GetScrollProvider()->InitializeScrollbars(
        this,
        descriptor.GetOverflowX() == OverflowModes::Scroll,
        descriptor.GetOverflowY() == OverflowModes::Scroll,
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
    AssertWindowInitialized();

    if (drawChildren)
    {
        // Important to draw child windows before drawing parent window.
        // Otherwise, WM_CLIPCHILDREN style will cause redraw of the parent window after the child windows draw.
        DrawChildWindows();
    }

    HDC deviceContext = GetDC(windowHandle);
    AssertCriticalWinApiResult(deviceContext);
    deviceContextBuffer->Render(deviceContext, windowSize);
    AssertCriticalWinApiResult(ReleaseDC(windowHandle, deviceContext));
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

    if (descriptor.GetOverflowX() == OverflowModes::Scroll)
    {
        scrollStyle |= WS_HSCROLL;
    }

    if (descriptor.GetOverflowY() == OverflowModes::Scroll)
    {
        scrollStyle |= WS_VSCROLL;
    }

    return scrollStyle;
}

Size Window::GetSize(bool downscale) const
{
    return !downscale ? windowSize : context->GetScaleProvider()->Downscale(windowSize);
}

Size Window::GetAvailableClientSize(bool downscale) const
{
    RECT clientRect;
    AssertCriticalWinApiResult(GetClientRect(windowHandle, &clientRect));

    ScrollProvider* scrollProvider = context->GetScrollProvider();
    Size availablClientSize = Size(
        clientRect.right + scrollProvider->GetScrollBarSize(this, ScrollBars::Vertical),
        clientRect.bottom + scrollProvider->GetScrollBarSize(this, ScrollBars::Horizontal));

    RECT windowRect;
    AssertCriticalWinApiResult(GetWindowRect(windowHandle, &windowRect));
    Size currentWindowSize = Size(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);

    double scaleFactor = double(windowRect.right - windowRect.left) / windowSize.Width;

    int horizontalFrame = roundToInt((currentWindowSize.Width - availablClientSize.Width) * scaleFactor);
    int verticalFrame = roundToInt((currentWindowSize.Height - availablClientSize.Height) * scaleFactor);

    Size scaledClientSize = Size(windowSize.Width - horizontalFrame, windowSize.Height - verticalFrame);

    return downscale
        ? context->GetScaleProvider()->Downscale(scaledClientSize)
        : scaledClientSize;
}

Size Window::GetContentSize() const
{
    return contentSize;
}

Point Window::GetPosition(bool downscale) const
{
    ScaleProvider* scaleProvider = context->GetScaleProvider();
    return !downscale ? position : Point(scaleProvider->Downscale(position.X), scaleProvider->Downscale(position.Y));
}

Rect Window::GetBoundingRect(bool downscale) const
{
    Point positon = GetPosition(downscale);
    Size size = GetSize(downscale);
    return Rect(positon, size);
}

void Window::MakeVisible()
{
    isVisible = true;
}

void Window::MakeHidden()
{
    isVisible = false;
}

bool Window::IsVisible() const
{
    return isVisible;
}

void Window::Show()
{
    AssertWindowInitialized();
    MakeVisible();
    ShowWindow(GetHandle(), SW_SHOW);
}

void Window::Hide()
{
    AssertWindowInitialized();
    MakeHidden();
    ShowWindow(GetHandle(), SW_HIDE);
}

LRESULT Window::ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    try
    {
        ExceptionHelper::SetupStructuredExceptionsTranslation();
        return WindowProcedure(message, wParam, lParam);
    }
    catch (const SelectedTextTranslateException& error)
    {
        ExceptionHelper::HandleNonFatalException(context->GetLogger(), context->GetErrorHandler(), L"Error occurred.", error);
    }
    catch (...)
    {
        context->GetErrorHandler()->HandleFatalException();
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
            return CallBaseWindowProcedure(message, wParam, lParam);
    }

    return 0;
}

void Window::AssertWindowInitialized() const
{
    if (windowState == WindowStates::New)
    {
        throw SelectedTextTranslateFatalException(L"Window has not been initialized.");
    }
}

void Window::AssertWindowNotInitialized() const
{
    if (windowState != WindowStates::New)
    {
        throw SelectedTextTranslateFatalException(L"Window has been already initialized.");
    }
}

Window::~Window()
{
    delete deviceContextBuffer;
    DestroyChildWindows(activeChildWindows);
    DestroyChildWindows(destroyBeforeDrawList);

    windowState = WindowStates::Destroyed;
}
