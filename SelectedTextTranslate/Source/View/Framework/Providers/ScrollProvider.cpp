#include "View\Framework\Providers\ScrollProvider.h"

ScrollProvider::ScrollProvider()
{
    this->scrollCharX = 8;
    this->scrollCharY = 20;

    this->verticalScrollBarWidth = GetSystemMetrics(SM_CYVSCROLL);
    this->horizontalScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
}

void ScrollProvider::InitializeScrollbars(
    Window* window,
    bool initializeHorizontalScroll,
    bool initializeVerticalScroll,
    int initialVerticalScrollPosition,
    int initialHorizontalScrollPostion) const
{
    if(!initializeVerticalScroll && !initializeHorizontalScroll)
    {
        return;
    }

    Size clientSize = window->GetAvailableClientSize();
    Size contentSize = window->GetContentSize();
    bool horizontalScrollVisible = initializeHorizontalScroll && clientSize.Width < contentSize.Width;
    bool verticalScrollVisible = initializeVerticalScroll && clientSize.Height < contentSize.Height;

    if(horizontalScrollVisible)
    {
        clientSize.Height -= horizontalScrollBarHeight;
    }

    if (verticalScrollVisible)
    {
        clientSize.Width -= verticalScrollBarWidth;
    }

    if(initializeHorizontalScroll)
    {
        InitializeScrollbar(window->GetHandle(), contentSize.Width, clientSize.Width, ScrollBars::Horizontal, initialHorizontalScrollPostion);
    }

    if(initializeVerticalScroll)
    {
        InitializeScrollbar(window->GetHandle(), contentSize.Height, clientSize.Height, ScrollBars::Vertical, initialVerticalScrollPosition);
    }
}

void ScrollProvider::InitializeScrollbar(HWND windowHandle, int contentDimension, int windowDimension, ScrollBars scrollBar, int initialPosition) const
{
    int scrollChar = GetScrollChar(scrollBar);
    int numberOfLines = int(ceil(contentDimension * 1.0 / scrollChar));
    int availableLines = int(ceil(windowDimension * 1.0 / scrollChar));

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

int ScrollProvider::GetCurrentScrollPostion(Window* window, ScrollBars scrollBar) const
{
    SCROLLINFO scrollInfo = GetWindowScrollInfo(window, scrollBar);
    return scrollInfo.nPos;
}

int ScrollProvider::GetCurrentScrollOffset(Window* window, ScrollBars scrollBar) const
{
    if(!IsScrollBarVisible(window, scrollBar))
    {
        return 0;
    }

    return GetCurrentScrollPostion(window, scrollBar) * GetScrollChar(scrollBar);
}

void ScrollProvider::ResetScrollsPosition(Window* window) const
{
    SCROLLINFO verticalScrollInfo = GetWindowScrollInfo(window, ScrollBars::Vertical);
    int scrollAmountVertical = verticalScrollInfo.nPos * scrollCharY;
    
    SCROLLINFO horizontalScrollInfo = GetWindowScrollInfo(window, ScrollBars::Horizontal);
    int scrollAmountHorizontal = horizontalScrollInfo.nPos * scrollCharX;

    ScrollWindowEx(
        window->GetHandle(),
        scrollAmountHorizontal,
        scrollAmountVertical,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        SW_SCROLLCHILDREN);

    window->Draw(true);
}

bool ScrollProvider::IsScrollBarVisible(const Window* window, ScrollBars scrollBar) const
{
    SCROLLBARINFO scrollBarInfo;
    scrollBarInfo.cbSize = sizeof SCROLLBARINFO;

    GetScrollBarInfo(
        window->GetHandle(),
        scrollBar == ScrollBars::Vertical ? OBJID_VSCROLL : OBJID_HSCROLL,
        &scrollBarInfo);

    return (scrollBarInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE) != STATE_SYSTEM_INVISIBLE;
}

int ScrollProvider::GetScrollBarSize(const Window* window, ScrollBars scrollBar) const
{
    if (IsScrollBarVisible(window, scrollBar))
    {
        return scrollBar == ScrollBars::Vertical
            ? verticalScrollBarWidth
            : horizontalScrollBarHeight;
    }

    return 0;
}

void ScrollProvider::ProcessScroll(Window* window, WPARAM wParam, LPARAM lParam, ScrollBars scrollBar) const
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

void ScrollProvider::SetScrollPosition(Window* window, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset) const
{
    scrollInfo.fMask = SIF_POS;
    SetScrollInfo(window->GetHandle(), (int)scrollBar, &scrollInfo, TRUE);
    GetScrollInfo(window->GetHandle(), (int)scrollBar, &scrollInfo);

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

        ScrollWindowEx(
            window->GetHandle(),
            scrollAmountHorizontal,
            scrollAmountVertical,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            SW_SCROLLCHILDREN);

        window->Draw(true);
    }
}

SCROLLINFO ScrollProvider::GetWindowScrollInfo(Window* window, ScrollBars scrollBar) const
{
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(scrollInfo);
    scrollInfo.fMask = SIF_ALL;
    GetScrollInfo(window->GetHandle(), (int)scrollBar, &scrollInfo);

    return scrollInfo;
}

int ScrollProvider::GetScrollChar(ScrollBars scrollBar) const
{
    return scrollBar == ScrollBars::Vertical
        ? scrollCharY
        : scrollCharX;
}

ScrollProvider::~ScrollProvider()
{
}
