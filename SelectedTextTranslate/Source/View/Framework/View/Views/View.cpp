#include "View\Framework\View\Views\View.h"
#include "View\Framework\View\Views\ChildView.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

View::View(ViewContext* context)
    : NativeWindowHolder(context->GetInstance())
{
    if(context == nullptr)
    {
        throw SelectedTextTranslateFatalException(L"View context must be provided.");
    }

    this->context = context;
    this->descriptor = WindowDescriptor();
    this->nativeStateDescriptor = WindowNativeStateDescriptor();
    this->activeChildViews = vector<View*>();
    this->destroyBeforeDrawList = vector<View*>();

    this->className = nullptr;
    this->deviceContextBuffer = nullptr;

    this->viewState = ViewStates::New;
}

WindowDescriptor View::GetDescriptor() const
{
    return descriptor;
}

void View::SetDescriptor(WindowDescriptor descriptor)
{
    AssertViewNotInitialized();
    this->descriptor = descriptor;
}

void View::Initialize()
{
    if(descriptor.IsEmpty())
    {
        throw SelectedTextTranslateException(L"Descriptor must be set for window");
    }

    deviceContextBuffer = new DeviceContextBuffer(context->GetDeviceContextProvider(), descriptor.GetSize());

    nativeStateDescriptor.SetSize(descriptor.GetSize());
    nativeStateDescriptor.SetPosition(descriptor.GetPosition());
    nativeStateDescriptor.MakeVisible();
    contentSize = Size(0, 0);

    NativeWindowHolder::Initialize();
    viewState = ViewStates::Initialized;
}

void View::InitializeAndRender(bool preserveScrolls)
{
    Initialize();
    Render(preserveScrolls);
}

void View::Render(bool preserveScrolls)
{
    AssertViewInitialized();

    RenderingContext* renderingContext = context->GetRenderingContext();

    renderingContext->BeginRender(this);
    viewState = ViewStates::Rendering;

    contentSize = RenderToBuffer();
    nativeStateDescriptor.SetSize(descriptor.GetSize());

    if (descriptor.GetOverflowX() == OverflowModes::Stretch && contentSize.GetWidth() > descriptor.GetSize().GetWidth())
    {
        nativeStateDescriptor.EnsureWidth(contentSize.GetWidth());
    }

    if (descriptor.GetOverflowY() == OverflowModes::Stretch && contentSize.GetHeight() > descriptor.GetSize().GetHeight())
    {
        nativeStateDescriptor.EnsureHeight(contentSize.GetHeight());
    }

    viewState = ViewStates::Rendered;

    if (renderingContext->IsRenderingRoot(this))
    {
        ApplyNativeState(preserveScrolls);
    }

    renderingContext->EndRender(this);
}

Size View::RenderToBuffer()
{
    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    Size renderedSize = RenderContent(renderer);

    Size deviceContextBufferSize = deviceContextBuffer->GetSize();

    const int requiredDcWidth = descriptor.GetOverflowX() != OverflowModes::Fixed && renderedSize.GetWidth() > deviceContextBufferSize.GetWidth()
        ? renderedSize.GetWidth()
        : deviceContextBufferSize.GetWidth();
    const int requiredDcHeight = descriptor.GetOverflowY() != OverflowModes::Fixed && renderedSize.GetHeight() > deviceContextBufferSize.GetHeight()
        ? renderedSize.GetHeight()
        : deviceContextBufferSize.GetHeight();

    Size requiredDcSize(requiredDcWidth, requiredDcHeight);

    if (!requiredDcSize.Equals(deviceContextBufferSize))
    {
        deviceContextBuffer->Resize(requiredDcSize);
    }

    renderer->Render(deviceContextBuffer);

    context->GetRenderingContext()->ReleaseRenderer(renderer);

    return renderedSize;
}

void View::ApplyNativeState(bool preserveScrolls)
{
    // Child windows should be destroyed first
    DestroyChildViews(destroyBeforeDrawList);

    ApplyViewPosition(preserveScrolls);

    // Important to draw child windows first
    for (size_t i = 0; i < activeChildViews.size(); ++i)
    {
        View* childView = activeChildViews[i];
        if (childView->IsVisible())
        {
            childView->Show();
        }
        else
        {
            childView->Hide();
        }

        childView->ApplyNativeState(preserveScrolls);
    }

    Draw(false);

    viewState = ViewStates::Drawn;
}

void View::ApplyViewPosition(bool preserveScrolls)
{
    int verticalScrollPosition = 0;
    int horizontalScrollPosition = 0;
    if (preserveScrolls)
    {
        verticalScrollPosition = context->GetScrollProvider()->GetCurrentScrollPostion(this, ScrollBars::Vertical);
        horizontalScrollPosition = context->GetScrollProvider()->GetCurrentScrollPostion(this, ScrollBars::Horizontal);
    }

    Point offset = GetInitialViewOffset();
    AssertCriticalWinApiResult(MoveWindow(
        windowHandle,
        descriptor.GetPosition().GetX() - offset.GetX(),
        descriptor.GetPosition().GetY() - offset.GetY(),
        nativeStateDescriptor.GetSize().GetWidth(),
        nativeStateDescriptor.GetSize().GetHeight(),
        FALSE));

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

Point View::GetInitialViewOffset()
{
    return Point(0, 0);
}

void View::Draw(bool drawChildren)
{
    AssertViewInitialized();

    if (drawChildren)
    {
        // Important to draw child windows before drawing parent window.
        // Otherwise, WM_CLIPCHILDREN style will cause redraw of the parent window after the child windows draw.
        DrawChildViews();
    }

    HDC deviceContext = GetDC(windowHandle);
    AssertCriticalWinApiResult(deviceContext);
    deviceContextBuffer->Render(deviceContext, GetSize());
    AssertCriticalWinApiResult(ReleaseDC(windowHandle, deviceContext));
}

void View::DrawChildViews()
{
    for (size_t i = 0; i < activeChildViews.size(); ++i)
    {
        View* childView = activeChildViews[i];
        childView->Draw(true);
    }
}

void View::AddChildView(View* childView)
{
    activeChildViews.push_back(childView);
}

void View::DestroyChildViews()
{
    destroyBeforeDrawList.insert(destroyBeforeDrawList.end(), activeChildViews.begin(), activeChildViews.end());
    activeChildViews = vector<View*>();
}

void View::DestroyChildViews(vector<View*>& childViews) const
{
    for (size_t i = 0; i < childViews.size(); ++i)
    {
        delete childViews[i];
    }

    childViews.clear();
    childViews.resize(0);
}

void View::Resize()
{
    // By default, do nothing on resize.
}

DWORD View::GetScrollStyle() const
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

Size View::GetSize() const
{
    return nativeStateDescriptor.GetSize();
}

Size View::GetClientSize() const
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

    // Important to compute the future window size in case window has been scaled but changes not yet applied
    double scaleFactor = double(windowRect.right - windowRect.left) / currentWindowSize.GetWidth();

    int horizontalFrame = roundToInt((currentWindowSize.GetWidth() - availablClientSize.GetWidth()) * scaleFactor);
    int verticalFrame = roundToInt((currentWindowSize.GetHeight() - availablClientSize.GetHeight()) * scaleFactor);

    Size scaledClientSize = Size(currentWindowSize.GetWidth() - horizontalFrame, currentWindowSize.GetHeight() - verticalFrame);

    return scaledClientSize;
}

Size View::GetContentSize() const
{
    return contentSize;
}

Point View::GetPosition() const
{
    return nativeStateDescriptor.GetPosition();
}

Rect View::GetBoundingRect() const
{
    Point positon = GetPosition();
    Size size = GetSize();
    return Rect(positon, size);
}

void View::MakeVisible()
{
    nativeStateDescriptor.MakeVisible();
}

void View::MakeHidden()
{
    nativeStateDescriptor.MakeHidden();
}

bool View::IsVisible() const
{
    return nativeStateDescriptor.IsVisible();
}

void View::Show()
{
    AssertViewInitialized();
    MakeVisible();
    ShowWindow(GetHandle(), SW_SHOW);
}

void View::Hide()
{
    AssertViewInitialized();
    MakeHidden();
    ShowWindow(GetHandle(), SW_HIDE);
}

LRESULT View::ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT View::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
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

void View::AssertViewInitialized() const
{
    if (viewState == ViewStates::New)
    {
        throw SelectedTextTranslateFatalException(L"View has not been initialized.");
    }
}

void View::AssertViewNotInitialized() const
{
    if (viewState != ViewStates::New)
    {
        throw SelectedTextTranslateFatalException(L"View has been already initialized.");
    }
}

View::~View()
{
    delete deviceContextBuffer;

    DestroyChildViews(activeChildViews);
    DestroyChildViews(destroyBeforeDrawList);

    viewState = ViewStates::Destroyed;
}
