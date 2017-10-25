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

    this->ParentView = parentView;
    this->IsLayered = false;

    parentView->AddChildView(this);
}

void ChildView::Initialize()
{
    View::Initialize();

    Point offset = GetInitialViewOffset();

    Handle = CreateWindow(
        ClassName,
        nullptr,
        WS_CHILD | WS_CLIPCHILDREN,
        LayoutDescriptor.GetPosition().GetX() - offset.GetX(),
        LayoutDescriptor.GetPosition().GetY() - offset.GetY(),
        LayoutDescriptor.GetSize().GetWidth(),
        LayoutDescriptor.GetSize().GetHeight(),
        ParentView->GetHandle(),
        nullptr,
        Context->GetInstance(),
        nullptr);

    AssertCriticalWinApiResult(Handle);

    SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR)this);

    if(IsLayered)
    {
        SetWindowLongPtr(Handle, GWL_EXSTYLE, GetWindowLongPtr(Handle, GWL_EXSTYLE) | WS_EX_LAYERED);
        AssertCriticalWinApiResult(SetLayeredWindowAttributes(Handle, 0, 255, LWA_ALPHA));
    }
}

void ChildView::EnableLayeredMode()
{
    AssertViewNotInitialized();
    this->IsLayered = true;
}

Point ChildView::GetInitialViewOffset()
{
    int offsetY = ScrollProvider->GetCurrentScrollOffset(ParentView, ScrollBars::Vertical);
    int offsetX = ScrollProvider->GetCurrentScrollOffset(ParentView, ScrollBars::Horizontal);

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
        AssertCriticalWinApiResult(GetWindowRect(Handle, &rcWindow));
        AssertCriticalWinApiResult(MoveWindow(Handle, pos.x, pos.y, ViewStateDescriptor.GetSize().GetWidth(), ViewStateDescriptor.GetSize().GetHeight(), FALSE));
        ViewStateDescriptor.SetPosition(Point(pos.x, pos.y));

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