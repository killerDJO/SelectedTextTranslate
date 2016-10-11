#include "Windows\Framework\Providers\ScrollProvider.h"

ScrollProvider::ScrollProvider()
{
    this->scrollCharX = 8;
    this->scrollCharY = 20;

    this->verticalScrollBarWidth = GetSystemMetrics(SM_CYVSCROLL);
    this->horizontalScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
}

void ScrollProvider::InitializeScrollbars(Window* window, bool initializeHorizontalScroll, bool initializeVerticalScroll) const
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
        clientSize.Height -= GetHorizontalScrollBarHeight();
    }

    if (verticalScrollVisible)
    {
        clientSize.Width -= GetVerticalScrollBarWidth();
    }

    if(initializeHorizontalScroll)
    {
        InitializeScrollbar(window->GetHandle(), contentSize.Width, clientSize.Width, scrollCharX, ScrollBars::Horizontal);
    }

    if(initializeVerticalScroll)
    {
        InitializeScrollbar(window->GetHandle(), contentSize.Height, clientSize.Height, scrollCharY, ScrollBars::Vertical);
    }
}

void ScrollProvider::InitializeScrollbar(HWND windowHandle, int contentDimension, int windowDimension, int scrollChar, ScrollBars scrollBar) const
{
    int numberOfLines = int(ceil(contentDimension * 1.0 / scrollChar));
    int availableLines = int(ceil(windowDimension * 1.0 / scrollChar));

    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nPos = 0;
    si.nMax = numberOfLines - 1;
    si.nPage = availableLines;
    SetScrollInfo(windowHandle, (int)scrollBar, &si, TRUE);
}

int ScrollProvider::GetScrollPosition(Window* window, ScrollBars scrollBar) const
{
    SCROLLINFO scrollInfo = GetWindowScrollInfo(window, scrollBar);
    return scrollInfo.nPos;
}

void ScrollProvider::SetScrollPosition(Window* window, ScrollBars scrollBar, int position) const
{
    SCROLLINFO scrollInfo = GetWindowScrollInfo(window, scrollBar);
    int scrollOffset = scrollInfo.nPos;

    scrollInfo.nPos = min(scrollInfo.nMax - scrollInfo.nPage + 1, (unsigned)max(position, 0));

    SetScrollPosition(window, scrollInfo, scrollBar, scrollOffset, scrollCharY);
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

    window->Redraw();
}

int ScrollProvider::GetVerticalScrollBarWidth() const
{
    return verticalScrollBarWidth;
}

int ScrollProvider::GetHorizontalScrollBarHeight() const
{
    return horizontalScrollBarHeight;
}

void ScrollProvider::ProcessVerticalScroll(Window* window, WPARAM wParam, LPARAM lParam) const
{
    ProcessScroll(window, wParam, lParam, scrollCharY, ScrollBars::Vertical);
}

void ScrollProvider::ProcessHorizontalScroll(Window* window, WPARAM wParam, LPARAM lParam) const
{
    ProcessScroll(window, wParam, lParam, scrollCharX, ScrollBars::Horizontal);
}

void ScrollProvider::ProcessScroll(Window* window, WPARAM wParam, LPARAM lParam, int scrollChar, ScrollBars scrollBar) const
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

    SetScrollPosition(window, scrollInfo, scrollBar, scrollOffset, scrollChar);
}

void ScrollProvider::SetScrollPosition(Window* window, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset, int scrollChar) const
{
    scrollInfo.fMask = SIF_POS;
    SetScrollInfo(window->GetHandle(), (int)scrollBar, &scrollInfo, TRUE);
    GetScrollInfo(window->GetHandle(), (int)scrollBar, &scrollInfo);

    if (scrollInfo.nPos != scrollOffset)
    {
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

        window->Redraw();
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

ScrollProvider::~ScrollProvider()
{
}