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
        AssertCriticalWinApiResult(SetLayeredWindowAttributes(Window->GetHandle(), 0, 255, LWA_ALPHA));
    }
}

void ChildView::EnableLayeredMode()
{
    AssertViewNotInitialized();
    this->IsLayered = true;
}

void ChildView::SpecifyWindow(NativeWindowHolder* window)
{
    window
        ->AddStyles(WS_CHILD)
        ->AddExtendedStyles(IsLayered ? WS_EX_LAYERED : 0)
        ->SetParent(ParentView->GetWindow()->GetHandle())
        // This message should be processed in order to enable scrolling.
        // It's sent to child windows after ScrollWindow on parent is called.
        ->SetMessageHandler(WM_MOVE, bind(&ChildView::ProcessMove, this, _1, _2))
        ->SetMessageHandler(WM_LBUTTONDOWN, bind(&ChildView::ProcessButtonDown, this, _1, _2));
}

Point ChildView::GetInitialViewOffset() const
{
    int offsetY = ScrollProvider->GetCurrentScrollOffset(ParentView, ScrollBars::Vertical);
    int offsetX = ScrollProvider->GetCurrentScrollOffset(ParentView, ScrollBars::Horizontal);

    return Point(offsetX, offsetY);
}

LRESULT ChildView::ProcessMove(WPARAM wParam, LPARAM lParam)
{
    POINTS pos = MAKEPOINTS(lParam);
    Window->Move(Rect(pos.x, pos.y, ViewState->GetViewSize().GetWidth(), ViewState->GetViewSize().GetHeight()));
    ViewState->SetPosition(Point(pos.x, pos.y));
    return TRUE;
}

LRESULT ChildView::ProcessButtonDown(WPARAM wParam, LPARAM lParam)
{
    Window->Focus();
    return TRUE;
}