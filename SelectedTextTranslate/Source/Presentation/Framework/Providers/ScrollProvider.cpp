#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include <winuser.h>

ScrollProvider::ScrollProvider()
{
    this->scrollCharX = 8;
    this->scrollCharY = 20;

    this->verticalScrollBarWidth = GetSystemMetrics(SM_CYVSCROLL);
    this->horizontalScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
}

void ScrollProvider::InitializeScrollbars(
    View* view,
    bool initializeHorizontalScroll,
    bool initializeVerticalScroll,
    int initialVerticalScrollPosition,
    int initialHorizontalScrollPostion) const
{
    if (!initializeVerticalScroll && !initializeHorizontalScroll)
    {
        return;
    }

    Size clientSize = view->GetAvailableClientSize();

    Size contentSize = Size(
        view->GetContentSize().GetWidth() - verticalScrollBarWidth,
        view->GetContentSize().GetHeight() - horizontalScrollBarHeight
    );

    bool horizontalScrollWillBeShown = initializeHorizontalScroll && clientSize.GetWidth() < contentSize.GetWidth();
    bool verticalScrollWillBeShown = initializeVerticalScroll && clientSize.GetHeight() < contentSize.GetHeight();

    // Clien side might be reduced because of scroll bars
    if (horizontalScrollWillBeShown)
    {
        clientSize = Size(
            clientSize.GetWidth(),
            clientSize.GetHeight() - horizontalScrollBarHeight);
    }

    if (verticalScrollWillBeShown)
    {
        clientSize = Size(
            clientSize.GetWidth() - verticalScrollBarWidth,
            clientSize.GetHeight());
    }

    if (initializeHorizontalScroll)
    {
        InitializeScrollbar(view->GetWindow()->GetHandle(), contentSize.GetWidth(), clientSize.GetWidth(), ScrollBars::Horizontal, initialHorizontalScrollPostion);
    }

    if (initializeVerticalScroll)
    {
        InitializeScrollbar(view->GetWindow()->GetHandle(), contentSize.GetHeight(), clientSize.GetHeight(), ScrollBars::Vertical, initialVerticalScrollPosition);
    }
}

void ScrollProvider::HideScrollbars(View* view) const
{
    InitializeScrollbar(view->GetWindow()->GetHandle(), 0, 1, ScrollBars::Horizontal, 0);
    InitializeScrollbar(view->GetWindow()->GetHandle(), 0, 1, ScrollBars::Vertical, 0);
}

LRESULT ScrollProvider::ProcessScrollMessages(View* view, UINT message, WPARAM wParam, LPARAM lParam) const
{
    if (message == WM_HSCROLL && IsScrollBarVisible(view, ScrollBars::Horizontal))
    {
        ProcessScroll(view, wParam, lParam, ScrollBars::Horizontal);
        return true;
    }

    if (message == WM_VSCROLL && IsScrollBarVisible(view, ScrollBars::Vertical))
    {
        ProcessScroll(view, wParam, lParam, ScrollBars::Vertical);
        return true;
    }

    if (message == WM_MOUSEWHEEL)
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta < 0)
        {
            SendMessage(view->GetWindow()->GetHandle(), WM_VSCROLL, SB_LINEDOWN, NULL);
        }
        else
        {
            SendMessage(view->GetWindow()->GetHandle(), WM_VSCROLL, SB_LINEUP, NULL);
        }

        return true;
    }

    return false;
}

void ScrollProvider::InitializeScrollbar(HWND windowHandle, int contentDimension, int windowDimension, ScrollBars scrollBar, int initialPosition) const
{
    int scrollChar = GetScrollChar(scrollBar);
    int numberOfLines = roundToInt(contentDimension * 1.0 / scrollChar);
    int availableLines = roundToInt(windowDimension * 1.0 / scrollChar);

    // There might be amount of content which is not part of the scroll because it's not evenly divided by scroll char
    // It is be spread between rest of scroll positions by scrolling logic
    int maxAvailableScrollAmount = (numberOfLines - availableLines) * scrollChar;
    int neededScrollAmount = contentDimension - windowDimension;
    int nonScrolledAmount = neededScrollAmount - maxAvailableScrollAmount;
    SetWindowLongPtr(windowHandle, (int)scrollBar * sizeof LONG_PTR, nonScrolledAmount);

    int availableInitialScrollPosition = min(numberOfLines - availableLines, initialPosition);

    SCROLLINFO scrollinfo;
    scrollinfo.cbSize = sizeof scrollinfo;
    scrollinfo.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    scrollinfo.nMin = 0;
    scrollinfo.nPos = availableInitialScrollPosition;
    scrollinfo.nMax = numberOfLines - 1;
    scrollinfo.nPage = availableLines;
    SetScrollInfo(windowHandle, (int)scrollBar, &scrollinfo, TRUE);
}

int ScrollProvider::GetCurrentScrollPostion(View* view, ScrollBars scrollBar) const
{
    if (!IsScrollBarVisible(view, scrollBar))
    {
        return 0;
    }

    SCROLLINFO scrollInfo = GetWindowScrollInfo(view, scrollBar);
    return scrollInfo.nPos;
}

int ScrollProvider::GetCurrentScrollOffset(View* view, ScrollBars scrollBar) const
{
    return GetCurrentScrollPostion(view, scrollBar) * GetScrollChar(scrollBar);
}

bool ScrollProvider::IsScrollBarVisible(const View* view, ScrollBars scrollBar) const
{
    SCROLLBARINFO scrollBarInfo;
    scrollBarInfo.cbSize = sizeof SCROLLBARINFO;

    AssertCriticalWinApiResult(GetScrollBarInfo(
        view->GetWindow()->GetHandle(),
        scrollBar == ScrollBars::Vertical ? OBJID_VSCROLL : OBJID_HSCROLL,
        &scrollBarInfo));

    return (scrollBarInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE) != STATE_SYSTEM_INVISIBLE;
}

int ScrollProvider::GetScrollBarSize(const View* view, ScrollBars scrollBar) const
{
    if (IsScrollBarVisible(view, scrollBar))
    {
        return scrollBar == ScrollBars::Vertical
            ? verticalScrollBarWidth
            : horizontalScrollBarHeight;
    }

    return 0;
}

void ScrollProvider::ProcessScroll(View* view, WPARAM wParam, LPARAM lParam, ScrollBars scrollBar) const
{
    SCROLLINFO scrollInfo = GetWindowScrollInfo(view, scrollBar);
    int currentScrollPosition = scrollInfo.nPos;

    switch (LOWORD(wParam))
    {
    case SB_TOP:
        scrollInfo.nPos = scrollInfo.nMin;
        break;

    case SB_BOTTOM:
        scrollInfo.nPos = scrollInfo.nMax;
        break;

    case SB_LINEUP:
        scrollInfo.nPos -= 1;
        break;

    case SB_LINEDOWN:
        scrollInfo.nPos += 1;
        break;

    case SB_PAGEUP:
        scrollInfo.nPos -= scrollInfo.nPage;
        break;

    case SB_PAGEDOWN:
        scrollInfo.nPos += scrollInfo.nPage;
        break;

    case SB_THUMBTRACK:
        scrollInfo.nPos = scrollInfo.nTrackPos;
        break;

    default:
        break;
    }

    SetScrollPosition(view, scrollInfo, scrollBar, currentScrollPosition);
}

void ScrollProvider::SetScrollPosition(View* view, SCROLLINFO scrollInfo, ScrollBars scrollBar, int currentScrollPosition) const
{
    scrollInfo.fMask = SIF_POS;
    SetScrollInfo(view->GetWindow()->GetHandle(), (int)scrollBar, &scrollInfo, TRUE);
    AssertCriticalWinApiResult(GetScrollInfo(view->GetWindow()->GetHandle(), (int)scrollBar, &scrollInfo));

    if (scrollInfo.nPos != currentScrollPosition)
    {
        int scrollChar = GetScrollChar(scrollBar);

        int scrollAdjustment = ComputeScrollAdjustment(view->GetWindow()->GetHandle(), scrollInfo, scrollBar, currentScrollPosition);
        int scrollAmount = (currentScrollPosition - scrollInfo.nPos) * scrollChar + scrollAdjustment;

        int scrollAmountHorizontal = 0;
        int scrollAmountVertical = 0;

        if (scrollBar == ScrollBars::Vertical)
        {
            scrollAmountVertical = scrollAmount;
        }
        else
        {
            scrollAmountHorizontal = scrollAmount;
        }

        int scrollResult = ScrollWindowEx(
            view->GetWindow()->GetHandle(),
            scrollAmountHorizontal,
            scrollAmountVertical,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            SW_SCROLLCHILDREN);

        ExceptionHelper::ThrowOnWinapiError(scrollResult, true, ERROR);

        view->Draw(true);
    }
}

// Spread non-scrolled amount between scroll positions. Firts positions get the priority.
int ScrollProvider::ComputeScrollAdjustment(HWND windowHandle, SCROLLINFO scrollInfo, ScrollBars scrollBar, int currentScrollPosition) const
{
    int availbleLines = scrollInfo.nMax - scrollInfo.nPage + 1;
    int nonScrolledAmount = GetWindowLongPtr(windowHandle, (int)scrollBar * sizeof LONG_PTR);

    int commmonAdjustment = abs(nonScrolledAmount) / availbleLines;

    int lowerScrollPosition = min(currentScrollPosition, scrollInfo.nPos);
    int biggerScrollPosition = max(currentScrollPosition, scrollInfo.nPos);
    int totalAdjustment = 0;
    for(int position = lowerScrollPosition; position < biggerScrollPosition; ++position)
    {
        totalAdjustment += commmonAdjustment + (position < abs(nonScrolledAmount) % availbleLines ? 1 : 0);
    }

    int signedAdjustment = roundToInt(copysign(1, currentScrollPosition - scrollInfo.nPos) * copysign(1, nonScrolledAmount) * totalAdjustment);

    return signedAdjustment;
}

SCROLLINFO ScrollProvider::GetWindowScrollInfo(View* view, ScrollBars scrollBar) const
{
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(scrollInfo);
    scrollInfo.fMask = SIF_ALL;
    AssertCriticalWinApiResult(GetScrollInfo(view->GetWindow()->GetHandle(), (int)scrollBar, &scrollInfo));

    return scrollInfo;
}

int ScrollProvider::GetScrollChar(ScrollBars scrollBar) const
{
    return scrollBar == ScrollBars::Vertical
        ? scrollCharY
        : scrollCharX;
}