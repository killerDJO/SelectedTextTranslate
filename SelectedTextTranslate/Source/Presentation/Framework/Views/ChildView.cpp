#include "Presentation\Framework\Views\ChildView.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"

ChildView::ChildView(CommonContext* context, View* parentView)
    : View(context)
{
    if(parentView == nullptr)
    {
        throw SelectedTextTranslateFatalException(L"Parent window must be provided.");
    }

    this->parentView = parentView;
    this->isLayered = false;

    parentView->AddChildView(this);
}

void ChildView::Initialize()
{
    View::Initialize();

    Point offset = GetInitialViewOffset();

    windowHandle = CreateWindow(
        className,
        nullptr,
        WS_CHILD | WS_CLIPCHILDREN,
        descriptor.GetPosition().GetX() - offset.GetX(),
        descriptor.GetPosition().GetY() - offset.GetY(),
        descriptor.GetSize().GetWidth(),
        descriptor.GetSize().GetHeight(),
        parentView->GetHandle(),
        nullptr,
        context->GetInstance(),
        nullptr);

    AssertCriticalWinApiResult(windowHandle);

    SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)this);

    if(isLayered)
    {
        SetWindowLongPtr(windowHandle, GWL_EXSTYLE, GetWindowLongPtr(windowHandle, GWL_EXSTYLE) | WS_EX_LAYERED);
        AssertCriticalWinApiResult(SetLayeredWindowAttributes(windowHandle, 0, 255, LWA_ALPHA));
    }
}

void ChildView::EnableLayeredMode()
{
    AssertViewNotInitialized();
    this->isLayered = true;
}

Point ChildView::GetInitialViewOffset()
{
    ScrollProvider* scrollProvider = context->Get<ScrollProvider>();
    int offsetY = scrollProvider->GetCurrentScrollOffset(parentView, ScrollBars::Vertical);
    int offsetX = scrollProvider->GetCurrentScrollOffset(parentView, ScrollBars::Horizontal);

    return Point(offsetX, offsetY);
}

LRESULT ChildView::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    // This message should be processed in order to enable scrolling.
    // Its sent to child windows after ScrollWindow on parent is called.
    case WM_MOVE:
    {
        RECT rcWindow;
        POINTS pos = MAKEPOINTS(lParam);
        AssertCriticalWinApiResult(GetWindowRect(windowHandle, &rcWindow));
        AssertCriticalWinApiResult(MoveWindow(windowHandle, pos.x, pos.y, nativeStateDescriptor.GetSize().GetWidth(), nativeStateDescriptor.GetSize().GetHeight(), FALSE));
        nativeStateDescriptor.SetPosition(Point(pos.x, pos.y));

        return TRUE;
    }

    case WM_LBUTTONDOWN:
    {
        AssertCriticalWinApiResult(SetFocus(windowHandle));
        return TRUE;
    }

    default:
        return View::WindowProcedure(message, wParam, lParam);
    }
}