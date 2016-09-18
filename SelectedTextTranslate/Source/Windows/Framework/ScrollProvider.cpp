#include "Windows\Framework\ScrollProvider.h"

ScrollProvider::ScrollProvider()
{
    this->scrollCharX = 8;
    this->scrollCharY = 20;
}

void ScrollProvider::InitializeScrollbar(HWND hWindow, int contentSize, int windowSize, ScrollBars scrollBar)
{
    int scrollChar = scrollBar == ScrollBars::Vertical
        ? scrollCharY
        : scrollCharX;

    InitializeScrollbar(hWindow, windowSize, contentSize, scrollChar, scrollBar);
}

void ScrollProvider::InitializeScrollbar(HWND hWindow, int windowDimension, int contentDimension, int scrollChar, ScrollBars scrollBar)
{
    SCROLLINFO si;

    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nPos = 0;
    si.nMax = roundToInt(contentDimension / scrollChar);
    si.nPage = roundToInt(windowDimension / scrollChar);
    SetScrollInfo(hWindow, (int)scrollBar, &si, TRUE);
}

int ScrollProvider::GetScrollPosition(HWND hWindow, ScrollBars scrollBar)
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(hWindow, scrollBar);
    return scrollInfo.nPos;
}

void ScrollProvider::SetScrollPosition(HWND hWindow, ScrollBars scrollBar, int position)
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(hWindow, scrollBar);
    int scrollOffset = scrollInfo.nPos;

    scrollInfo.nPos = min(scrollInfo.nMax - scrollInfo.nPage + 1, (unsigned)max(position, 0));

    SetScrollPosition(hWindow, scrollInfo, scrollBar, scrollOffset, scrollCharY);
}

void ScrollProvider::ProcessVerticalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
    ProcessScroll(hWindow, wParam, lParam, scrollCharY, ScrollBars::Vertical);
}

void ScrollProvider::ProcessHorizontalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
    ProcessScroll(hWindow, wParam, lParam, scrollCharX, ScrollBars::Horizontal);
}

void ScrollProvider::ProcessScroll(HWND hWindow, WPARAM wParam, LPARAM lParam, int scrollChar, ScrollBars scrollBar)
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(hWindow, scrollBar);
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

    SetScrollPosition(hWindow, scrollInfo, scrollBar, scrollOffset, scrollChar);
}

void ScrollProvider::SetScrollPosition(HWND hWindow, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset, int scrollChar)
{
    scrollInfo.fMask = SIF_POS;
    SetScrollInfo(hWindow, (int)scrollBar, &scrollInfo, TRUE);
    GetScrollInfo(hWindow, (int)scrollBar, &scrollInfo);

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
            hWindow,
            scrollAmountHorizontal,
            scrollAmountVertical,
            NULL,
            NULL,
            NULL,
            NULL,
            SW_SCROLLCHILDREN);
    }
}

SCROLLINFO ScrollProvider::GetScrollBarInfo(HWND hWindow, ScrollBars scrollBar)
{
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(scrollInfo);
    scrollInfo.fMask = SIF_ALL;
    GetScrollInfo(hWindow, (int)scrollBar, &scrollInfo);

    return scrollInfo;
}

ScrollProvider::~ScrollProvider()
{
}