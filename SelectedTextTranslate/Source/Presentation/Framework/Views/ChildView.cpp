#include "Presentation\Framework\Views\ChildView.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"

ChildView::ChildView(ViewContext* context, View* parentView)
    : View(context)
{
    if (parentView == nullptr)
    {
        throw SelectedTextTranslateFatalException(L"Parent view must be provided.");
    }

    ParentView = parentView;
    IsLayered = false;

    parentView->AddChildView(this);
}

void ChildView::Initialize()
{
    View::Initialize();

    if (IsLayered)
    {
        AssertCriticalWinApiResult(SetLayeredWindowAttributes(Handle, 0, 255, LWA_ALPHA));
    }
}

void ChildView::EnableLayeredMode()
{
    AssertViewNotInitialized();
    this->IsLayered = true;
}

Point ChildView::GetInitialViewOffset() const
{
    int offsetY = ScrollProvider->GetCurrentScrollOffset(ParentView, ScrollBars::Vertical);
    int offsetX = ScrollProvider->GetCurrentScrollOffset(ParentView, ScrollBars::Horizontal);

    return Point(offsetX, offsetY);
}

DWORD ChildView::GetWindowStyle() const
{
    return View::GetWindowStyle() | WS_CHILD;
}

DWORD ChildView::GetExtendedWindowStyles() const
{
    return View::GetExtendedWindowStyles() | (IsLayered ? WS_EX_LAYERED : 0);
}

HWND ChildView::GetWindowParent() const
{
    return ParentView->GetHandle();
}

LRESULT ChildView::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    // This message should be processed in order to enable scrolling.
    // It's sent to child windows after ScrollWindow on parent is called.
    case WM_MOVE:
    {
        RECT rcWindow;
        POINTS pos = MAKEPOINTS(lParam);
        AssertCriticalWinApiResult(GetWindowRect(Handle, &rcWindow));
        AssertCriticalWinApiResult(MoveWindow(Handle, pos.x, pos.y, ViewState->GetViewSize().GetWidth(), ViewState->GetViewSize().GetHeight(), FALSE));
        ViewState->SetPosition(Point(pos.x, pos.y));

        return TRUE;
    }

    case WM_LBUTTONDOWN:
    {
        AssertCriticalWinApiResult(SetFocus(Handle));
        return TRUE;
    }

    default:
        return View::WindowProcedure(message, wParam, lParam);
    }
}
