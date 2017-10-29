#include "Presentation\Framework\Views\View.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

View::View(ViewContext* context)
    : NativeWindowHolder()
{
    if(context == nullptr)
    {
        throw SelectedTextTranslateFatalException(L"View context must be provided.");
    }

    Context = context;
    ScaleProvider = context->GetScaleProvider();
    ScrollProvider = context->GetScrollProvider();
    RenderingProvider = context->GetRenderingProvider();
    RenderingContext = context->GetRenderingContext();
    DeviceContextProvider = context->GetDeviceContextProvider();

    Layout = LayoutDescriptor();
    State = ViewStateDescriptor();
    activeChildViews = vector<View*>();
    destroyedChildViews = vector<View*>();

    ClassName = nullptr;
    DeviceContextBuffer = nullptr;
    Name = wstring();
}

void View::Initialize()
{
    if(Layout.IsEmpty())
    {
        throw SelectedTextTranslateException(L"Descriptor must be set for view");
    }

    DeviceContextBuffer = new ::DeviceContextBuffer(DeviceContextProvider, Layout.GetSize());

    State.SetSize(Layout.GetSize());
    State.SetPosition(Layout.GetPosition());
    State.MakeVisible();

    NativeWindowHolder::Initialize();
    State.SetViewState(ViewStates::Initialized);
}

void View::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
    // Reserve for scrolling
    windowClass->cbWndExtra = 2 * sizeof(LONG_PTR);
}

Rect View::GetWindowRectangle() const
{
    Point offset = GetInitialViewOffset();
    Point position = Point(
        Layout.GetPosition().GetX() - offset.GetX(),
        Layout.GetPosition().GetY() - offset.GetY());
    return Rect(position, Layout.GetSize());
}

Point View::GetInitialViewOffset() const
{
    return Point(0, 0);
}

DWORD View::GetWindowStyle() const
{
    int scrollStyle = 0;

    if (Layout.GetOverflowX() == OverflowModes::Scroll)
    {
        scrollStyle |= WS_HSCROLL;
    }

    if (Layout.GetOverflowY() == OverflowModes::Scroll)
    {
        scrollStyle |= WS_VSCROLL;
    }

    return NativeWindowHolder::GetWindowStyle() | scrollStyle | WS_CLIPCHILDREN;
}

void View::InitializeAndRender(bool preserveScrolls)
{
    Initialize();
    Render(preserveScrolls);
}

LayoutDescriptor View::GetLayout() const
{
    return Layout;
}

void View::Render(bool preserveScrolls)
{
    printf("Render: %ls, %ls\n", Name.c_str(), ClassName);
    AssertViewInitialized();

    RenderingContext->BeginRender(this);
    State.SetViewState(ViewStates::Rendering);

    Size contentSize = RenderToBuffer();
    State.SetSize(Layout.GetSize());

    if (Layout.GetOverflowX() == OverflowModes::Stretch && contentSize.GetWidth() > Layout.GetSize().GetWidth())
    {
        State.SetWidth(contentSize.GetWidth());
    }

    if (Layout.GetOverflowY() == OverflowModes::Stretch && contentSize.GetHeight() > Layout.GetSize().GetHeight())
    {
        State.SetHeight(contentSize.GetHeight());
    }

    State.SetViewState(ViewStates::Rendering);
    State.SetContentSize(contentSize);

    if (RenderingContext->IsRenderingRoot(this))
    {
        ApplyViewState(preserveScrolls);
    }

    RenderingContext->EndRender(this);
}

Size View::RenderToBuffer()
{
    Renderer* renderer = RenderingContext->GetRenderer();

    Size renderedSize = RenderContent(renderer);

    Size deviceContextBufferSize = DeviceContextBuffer->GetSize();

    const int requiredDcWidth = Layout.GetOverflowX() != OverflowModes::Fixed && renderedSize.GetWidth() > deviceContextBufferSize.GetWidth()
        ? renderedSize.GetWidth()
        : deviceContextBufferSize.GetWidth();
    const int requiredDcHeight = Layout.GetOverflowY() != OverflowModes::Fixed && renderedSize.GetHeight() > deviceContextBufferSize.GetHeight()
        ? renderedSize.GetHeight()
        : deviceContextBufferSize.GetHeight();

    Size requiredDcSize(requiredDcWidth, requiredDcHeight);

    if (!requiredDcSize.Equals(deviceContextBufferSize))
    {
        DeviceContextBuffer->Resize(requiredDcSize);
    }

    renderer->Render(DeviceContextBuffer);

    RenderingContext->ReleaseRenderer(renderer);

    return renderedSize;
}

void View::ApplyViewState(bool preserveScrolls)
{
    // Child windows should be destroyed first
    DestroyChildViews(destroyedChildViews);

    if (IsVisible())
    {
        Show();

        ApplyViewPosition(preserveScrolls);

        // Important to draw child windows first
        for (View* childView : activeChildViews)
        {
            childView->ApplyViewState(preserveScrolls);
        }

        Draw(false);
    }
    else
    {
        Hide();
    }
}

void View::ApplyViewPosition(bool preserveScrolls)
{
    int verticalScrollPosition = 0;
    int horizontalScrollPosition = 0;

    if (preserveScrolls)
    {
        verticalScrollPosition = ScrollProvider->GetCurrentScrollPostion(this, ScrollBars::Vertical);
        horizontalScrollPosition = ScrollProvider->GetCurrentScrollPostion(this, ScrollBars::Horizontal);
    }

    Point offset = GetInitialViewOffset();
    AssertCriticalWinApiResult(MoveWindow(
        Handle,
        Layout.GetPosition().GetX() - offset.GetX(),
        Layout.GetPosition().GetY() - offset.GetY(),
        State.GetSize().GetWidth(),
        State.GetSize().GetHeight(),
        FALSE));

    // Important to initialize scroll only after window has been moved
    ScrollProvider->InitializeScrollbars(
        this,
        Layout.GetOverflowX() == OverflowModes::Scroll,
        Layout.GetOverflowY() == OverflowModes::Scroll,
        verticalScrollPosition,
        horizontalScrollPosition);

    // Render scroll immediately
    SendMessage(Handle, WM_NCPAINT, NULL, NULL);
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

    HDC deviceContext = GetDC(Handle);
    AssertCriticalWinApiResult(deviceContext);
    DeviceContextBuffer->Render(deviceContext, State.GetSize());
    AssertCriticalWinApiResult(ReleaseDC(Handle, deviceContext));
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
    destroyedChildViews.insert(destroyedChildViews.end(), activeChildViews.begin(), activeChildViews.end());
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

Size View::GetAvailableClientSize() const
{
    Size currentClientSize = GetCurrentClientSize();

    // Do not count showed scrollbars towards client size
    Size availablClientSize = Size(
        currentClientSize.GetWidth() + ScrollProvider->GetScrollBarSize(this, ScrollBars::Vertical),
        currentClientSize.GetHeight() + ScrollProvider->GetScrollBarSize(this, ScrollBars::Horizontal));

    return availablClientSize;
}

Size View::GetCurrentClientSize() const
{
    RECT clientRect;
    AssertCriticalWinApiResult(GetClientRect(Handle, &clientRect));
    return Size(clientRect.right, clientRect.bottom);
}

Size View::GetContentSize() const
{
    return State.GetContentSize();
}

Rect View::GetBoundingRect() const
{
    return State.GetBoundingRect();
}

void View::MakeVisible()
{
    State.MakeVisible();
}

void View::MakeHidden()
{
    State.MakeHidden();
}

bool View::IsVisible() const
{
    return State.IsVisible();
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
        ExceptionHelper::HandleNonFatalException(Context->GetLogger(), Context->GetErrorHandler(), L"Error occurred.", error);
    }
    catch (...)
    {
        Context->GetErrorHandler()->HandleFatalException();
    }

    return -1;
}

LRESULT View::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    ScrollProvider->ProcessScrollMessages(this, message, wParam, lParam);

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
    if (State.GetViewState() == ViewStates::New)
    {
        throw SelectedTextTranslateFatalException(L"View has not been initialized.");
    }
}

void View::AssertViewNotInitialized() const
{
    if (State.GetViewState() != ViewStates::New)
    {
        throw SelectedTextTranslateFatalException(L"View has been already initialized.");
    }
}

View::~View()
{
    delete DeviceContextBuffer;

    DestroyChildViews(activeChildViews);
    DestroyChildViews(destroyedChildViews);

    State.SetViewState(ViewStates::Destroyed);
}