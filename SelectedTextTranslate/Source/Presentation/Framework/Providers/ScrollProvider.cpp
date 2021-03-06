#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ScrollProvider::ScrollProvider()
{
    this->scrollCharX = 8;
    this->scrollCharY = 20;

    this->verticalScrollBarWidth = GetSystemMetrics(SM_CYVSCROLL);
    this->horizontalScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
}

void ScrollProvider::InitializeScrollbars(
    View* window,
    bool initializeHorizontalScroll,
    bool initializeVerticalScroll,
    int initialVerticalScrollPosition,
    int initialHorizontalScrollPostion) const
{
    if (!initializeVerticalScroll && !initializeHorizontalScroll)
    {
        return;
    }

    Size clientSize = window->GetClientSize();

    Size contentSize = window->GetContentSize();
    contentSize = Size(
        contentSize.GetWidth() - GetScrollChar(ScrollBars::Vertical),
        contentSize.GetHeight() - GetScrollChar(ScrollBars::Horizontal));

    bool horizontalScrollVisible = initializeHorizontalScroll && clientSize.GetWidth() < contentSize.GetWidth();
    bool verticalScrollVisible = initializeVerticalScroll && clientSize.GetHeight() < contentSize.GetHeight();

    if (horizontalScrollVisible)
    {
        clientSize = Size(
            clientSize.GetWidth(),
            clientSize.GetHeight() - horizontalScrollBarHeight);
    }

    if (verticalScrollVisible)
    {
        clientSize = Size(
            clientSize.GetWidth() - verticalScrollBarWidth,
            clientSize.GetHeight());
    }

    if (initializeHorizontalScroll)
    {
        InitializeScrollbar(window->GetHandle(), contentSize.GetWidth(), clientSize.GetWidth(), ScrollBars::Horizontal, initialHorizontalScrollPostion);
    }

    if (initializeVerticalScroll)
    {
        InitializeScrollbar(window->GetHandle(), contentSize.GetHeight(), clientSize.GetHeight(), ScrollBars::Vertical, initialVerticalScrollPosition);
    }
}

void ScrollProvider::HideScrollbars(View* view) const
{
    InitializeScrollbar(view->GetHandle(), 0, 1, ScrollBars::Horizontal, 0);
    InitializeScrollbar(view->GetHandle(), 0, 1, ScrollBars::Vertical, 0);
}

void ScrollProvider::ProcessScrollMessages(View* view, UINT message, WPARAM wParam, LPARAM lParam) const
{
    if (message == WM_HSCROLL && view->GetLayout().GetOverflowX() == OverflowModes::Scroll && IsScrollBarVisible(view, ScrollBars::Horizontal))
    {
        ProcessScroll(view, wParam, lParam, ScrollBars::Horizontal);
    }

    if (message == WM_VSCROLL && view->GetLayout().GetOverflowY() == OverflowModes::Scroll && IsScrollBarVisible(view, ScrollBars::Vertical))
    {
        ProcessScroll(view, wParam, lParam, ScrollBars::Vertical);
    }

    if (message == WM_MOUSEWHEEL)
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta < 0)
        {
            SendMessage(view->GetHandle(), WM_VSCROLL, SB_LINEDOWN, NULL);
        }
        else
        {
            SendMessage(view->GetHandle(), WM_VSCROLL, SB_LINEUP, NULL);
        }
    }
}

void ScrollProvider::InitializeScrollbar(HWND windowHandle, int contentDimension, int windowDimension, ScrollBars scrollBar, int initialPosition) const
{
    int scrollChar = GetScrollChar(scrollBar);
    int numberOfLines = int(ceil(contentDimension * 1.0 / scrollChar));
    int availableLines = int(floor(windowDimension * 1.0 / scrollChar));

    int availableInitialScrollPosition = min(numberOfLines - availableLines, initialPosition);

    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nPos = availableInitialScrollPosition;
    si.nMax = numberOfLines - 1;
    si.nPage = availableLines;
    SetScrollInfo(windowHandle, (int)scrollBar, &si, TRUE);
}

int ScrollProvider::GetCurrentScrollPostion(View* window, ScrollBars scrollBar) const
{
    if (!IsScrollBarVisible(window, scrollBar))
    {
        return 0;
    }

    SCROLLINFO scrollInfo = GetWindowScrollInfo(window, scrollBar);
    return scrollInfo.nPos;
}

int ScrollProvider::GetCurrentScrollOffset(View* window, ScrollBars scrollBar) const
{
    if (!IsScrollBarVisible(window, scrollBar))
    {
        return 0;
    }

    return GetCurrentScrollPostion(window, scrollBar) * GetScrollChar(scrollBar);
}

bool ScrollProvider::IsScrollBarVisible(const View* window, ScrollBars scrollBar) const
{
    SCROLLBARINFO scrollBarInfo;
    scrollBarInfo.cbSize = sizeof SCROLLBARINFO;

    AssertCriticalWinApiResult(GetScrollBarInfo(
        window->GetHandle(),
        scrollBar == ScrollBars::Vertical ? OBJID_VSCROLL : OBJID_HSCROLL,
        &scrollBarInfo));

    return (scrollBarInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE) != STATE_SYSTEM_INVISIBLE;
}

int ScrollProvider::GetScrollBarSize(const View* window, ScrollBars scrollBar) const
{
    if (IsScrollBarVisible(window, scrollBar))
    {
        return scrollBar == ScrollBars::Vertical
            ? verticalScrollBarWidth
            : horizontalScrollBarHeight;
    }

    return 0;
}

void ScrollProvider::ProcessScroll(View* window, WPARAM wParam, LPARAM lParam, ScrollBars scrollBar) const
{
    SCROLLINFO scrollInfo = GetWindowScrollInfo(window, scrollBar);
    int scrollOffset = scrollInfo.nPos;

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

    SetScrollPosition(window, scrollInfo, scrollBar, scrollOffset);
}

Size ScrollProvider::AlignWithScrollingGrid(Size size) const
{
    int scrollCharX = GetScrollChar(ScrollBars::Horizontal);
    int scrollCharY = GetScrollChar(ScrollBars::Vertical);
    return Size(
        roundToInt(ceil((double)size.GetWidth() / scrollCharX) * scrollCharX),
        roundToInt(ceil((double)size.GetHeight() / scrollCharY) * scrollCharY));
}

void ScrollProvider::SetScrollPosition(View* window, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset) const
{
    scrollInfo.fMask = SIF_POS;
    SetScrollInfo(window->GetHandle(), (int)scrollBar, &scrollInfo, TRUE);
    AssertCriticalWinApiResult(GetScrollInfo(window->GetHandle(), (int)scrollBar, &scrollInfo));

    if (scrollInfo.nPos != scrollOffset)
    {
        int scrollChar = GetScrollChar(scrollBar);
        int scrollAmount = scrollChar * (scrollOffset - scrollInfo.nPos);
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
            window->GetHandle(),
            scrollAmountHorizontal,
            scrollAmountVertical,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            SW_SCROLLCHILDREN);

        ExceptionHelper::ThrowOnWinapiError(scrollResult, true, ERROR);

        window->Draw(true);
    }
}

SCROLLINFO ScrollProvider::GetWindowScrollInfo(View* window, ScrollBars scrollBar) const
{
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(scrollInfo);
    scrollInfo.fMask = SIF_ALL;
    AssertCriticalWinApiResult(GetScrollInfo(window->GetHandle(), (int)scrollBar, &scrollInfo));

    return scrollInfo;
}

int ScrollProvider::GetScrollChar(ScrollBars scrollBar) const
{
    return scrollBar == ScrollBars::Vertical
        ? scrollCharY
        : scrollCharX;
}