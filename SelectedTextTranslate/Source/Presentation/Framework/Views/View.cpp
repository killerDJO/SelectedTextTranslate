#include "Presentation\Framework\Views\View.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"

View::View(CommonContext* context)
    : NativeWindowHolder(context->GetInstance())
{
    if(context == nullptr)
    {
        throw SelectedTextTranslateFatalException(L"View context must be provided.");
    }

    this->Context = context;
    this->ScaleProvider = context->Get<::ScaleProvider>();
    this->ScrollProvider = context->Get<::ScrollProvider>();
    this->RenderingProvider = context->Get<::RenderingProvider>();
    this->RenderingContext = context->Get<::RenderingContext>();
    this->DeviceContextProvider = context->Get<::DeviceContextProvider>();

    this->LayoutDescriptor = ::LayoutDescriptor();
    this->ViewStateDescriptor = ::ViewStateDescriptor();
    this->ActiveChildViews = vector<View*>();
    this->destroyedChildViews = vector<View*>();

    this->ClassName = nullptr;
    this->DeviceContextBuffer = nullptr;
    this->ViewName = wstring();
}

void View::Initialize()
{
    if(LayoutDescriptor.IsEmpty())
    {
        throw SelectedTextTranslateException(L"Descriptor must be set for window");
    }

    DeviceContextBuffer = new ::DeviceContextBuffer(DeviceContextProvider, LayoutDescriptor.GetSize());

    ViewStateDescriptor.SetSize(LayoutDescriptor.GetSize());
    ViewStateDescriptor.SetPosition(LayoutDescriptor.GetPosition());
    ViewStateDescriptor.MakeVisible();

    NativeWindowHolder::Initialize();
    ViewStateDescriptor.SetViewState(ViewStates::Initialized);
}

void View::InitializeAndRender(bool preserveScrolls)
{
    Initialize();
    Render(preserveScrolls);
}

LayoutDescriptor View::GetLayout() const
{
    return LayoutDescriptor;
}

void View::Render(bool preserveScrolls)
{
    printf("Render: %ls, %ls\n", ViewName.c_str(), ClassName);
    AssertViewInitialized();

    RenderingContext->BeginRender(this);
    ViewStateDescriptor.SetViewState(ViewStates::Rendering);

    Size contentSize = RenderToBuffer();
    ViewStateDescriptor.SetSize(LayoutDescriptor.GetSize());

    if (LayoutDescriptor.GetOverflowX() == OverflowModes::Stretch && contentSize.GetWidth() > LayoutDescriptor.GetSize().GetWidth())
    {
        ViewStateDescriptor.EnsureWidth(contentSize.GetWidth());
    }

    if (LayoutDescriptor.GetOverflowY() == OverflowModes::Stretch && contentSize.GetHeight() > LayoutDescriptor.GetSize().GetHeight())
    {
        ViewStateDescriptor.EnsureHeight(contentSize.GetHeight());
    }

    ViewStateDescriptor.SetViewState(ViewStates::Rendering);
    ViewStateDescriptor.SetContentSize(contentSize);

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

    const int requiredDcWidth = LayoutDescriptor.GetOverflowX() != OverflowModes::Fixed && renderedSize.GetWidth() > deviceContextBufferSize.GetWidth()
        ? renderedSize.GetWidth()
        : deviceContextBufferSize.GetWidth();
    const int requiredDcHeight = LayoutDescriptor.GetOverflowY() != OverflowModes::Fixed && renderedSize.GetHeight() > deviceContextBufferSize.GetHeight()
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

    ApplyViewPosition(preserveScrolls);

    // Important to draw child windows first
    for (size_t i = 0; i < ActiveChildViews.size(); ++i)
    {
        View* childView = ActiveChildViews[i];
        if (childView->IsVisible())
        {
            childView->Show();
        }
        else
        {
            childView->Hide();
        }

        childView->ApplyViewState(preserveScrolls);
    }

    Draw(false);
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
        LayoutDescriptor.GetPosition().GetX() - offset.GetX(),
        LayoutDescriptor.GetPosition().GetY() - offset.GetY(),
        ViewStateDescriptor.GetSize().GetWidth(),
        ViewStateDescriptor.GetSize().GetHeight(),
        FALSE));

    // Important to initialize scroll only after window has been moved
    ScrollProvider->InitializeScrollbars(
        this,
        LayoutDescriptor.GetOverflowX() == OverflowModes::Scroll,
        LayoutDescriptor.GetOverflowY() == OverflowModes::Scroll,
        verticalScrollPosition,
        horizontalScrollPosition);

    // Render scroll immediately
    SendMessage(Handle, WM_NCPAINT, NULL, NULL);
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

    HDC deviceContext = GetDC(Handle);
    AssertCriticalWinApiResult(deviceContext);
    DeviceContextBuffer->Render(deviceContext, GetSize());
    AssertCriticalWinApiResult(ReleaseDC(Handle, deviceContext));
}

void View::DrawChildViews()
{
    for (size_t i = 0; i < ActiveChildViews.size(); ++i)
    {
        View* childView = ActiveChildViews[i];
        childView->Draw(true);
    }
}

void View::AddChildView(View* childView)
{
    ActiveChildViews.push_back(childView);
}

void View::DestroyChildViews()
{
    destroyedChildViews.insert(destroyedChildViews.end(), ActiveChildViews.begin(), ActiveChildViews.end());
    ActiveChildViews = vector<View*>();
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

    if (LayoutDescriptor.GetOverflowX() == OverflowModes::Scroll)
    {
        scrollStyle |= WS_HSCROLL;
    }

    if (LayoutDescriptor.GetOverflowY() == OverflowModes::Scroll)
    {
        scrollStyle |= WS_VSCROLL;
    }

    return scrollStyle;
}

Size View::GetSize() const
{
    return ViewStateDescriptor.GetSize();
}

Size View::GetClientSize() const
{
    RECT clientRect;
    AssertCriticalWinApiResult(GetClientRect(Handle, &clientRect));

    Size availablClientSize = Size(
        clientRect.right + ScrollProvider->GetScrollBarSize(this, ScrollBars::Vertical),
        clientRect.bottom + ScrollProvider->GetScrollBarSize(this, ScrollBars::Horizontal));

    RECT windowRect;
    AssertCriticalWinApiResult(GetWindowRect(Handle, &windowRect));
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
    return ViewStateDescriptor.GetContentSize();
}

Point View::GetPosition() const
{
    return ViewStateDescriptor.GetPosition();
}

Rect View::GetBoundingRect() const
{
    Point positon = GetPosition();
    Size size = GetSize();
    return Rect(positon, size);
}

void View::MakeVisible()
{
    ViewStateDescriptor.MakeVisible();
}

void View::MakeHidden()
{
    ViewStateDescriptor.MakeHidden();
}

bool View::IsVisible() const
{
    return ViewStateDescriptor.IsVisible();
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
        ExceptionHelper::HandleNonFatalException(Context->Get<Logger>(), Context->GetErrorHandler(), L"Error occurred.", error);
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
    if (ViewStateDescriptor.GetViewState() == ViewStates::New)
    {
        throw SelectedTextTranslateFatalException(L"View has not been initialized.");
    }
}

void View::AssertViewNotInitialized() const
{
    if (ViewStateDescriptor.GetViewState() != ViewStates::New)
    {
        throw SelectedTextTranslateFatalException(L"View has been already initialized.");
    }
}

View::~View()
{
    delete DeviceContextBuffer;

    DestroyChildViews(ActiveChildViews);
    DestroyChildViews(destroyedChildViews);

    ViewStateDescriptor.SetViewState(ViewStates::Destroyed);
}
