#include "Windows\Framework\ScrollProvider.h"

ScrollProvider::ScrollProvider(Renderer* renderer)
{
    this->renderer = renderer;
    this->scrollCharX = 8;
    this->scrollCharY = 20;
}

void ScrollProvider::InitializeScrollbars(HWND hWindow, int contentWidth, int contentHeight)
{
    RECT windowRect;
    GetWindowRect(hWindow, &windowRect);

    InitializeScrollbar(hWindow, windowRect.bottom - windowRect.top, contentHeight, scrollCharY, SB_VERT);
    InitializeScrollbar(hWindow, windowRect.right - windowRect.left, contentWidth, scrollCharX, SB_HORZ);
}

void ScrollProvider::InitializeScrollbar(HWND hWindow, int windowDimension, int contentDimension, int scrollChar, int nBar)
{
    SCROLLINFO si;

    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nPos = 0;
    si.nMax = roundToInt(contentDimension / scrollChar);
    si.nPage = roundToInt(windowDimension / scrollChar);
    SetScrollInfo(hWindow, nBar, &si, TRUE);
}

int ScrollProvider::GetScrollPosition(HWND hWindow, int nBar)
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(hWindow, nBar);
    return scrollInfo.nPos;
}

void ScrollProvider::SetScrollPosition(HWND hWindow, int nBar, int position)
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(hWindow, nBar);
    int scrollOffset = scrollInfo.nPos;

    scrollInfo.nPos = min(scrollInfo.nMax - scrollInfo.nPage + 1, (unsigned)max(position, 0));

    SetScrollPosition(hWindow, scrollInfo, nBar, scrollOffset, scrollCharY);
}

void ScrollProvider::ProcessVerticalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
    ProcessScroll(hWindow, wParam, lParam, scrollCharY, SB_VERT);
}

void ScrollProvider::ProcessHorizontalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
    ProcessScroll(hWindow, wParam, lParam, scrollCharX, SB_HORZ);
}

void ScrollProvider::ProcessScroll(HWND hWindow, WPARAM wParam, LPARAM lParam, int scrollChar, int nBar)
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(hWindow, nBar);
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

    SetScrollPosition(hWindow, scrollInfo, nBar, scrollOffset, scrollChar);
}

void ScrollProvider::SetScrollPosition(HWND hWindow, SCROLLINFO scrollInfo, int nBar, int scrollOffset, int scrollChar)
{
    scrollInfo.fMask = SIF_POS;
    SetScrollInfo(hWindow, nBar, &scrollInfo, TRUE);
    GetScrollInfo(hWindow, nBar, &scrollInfo);

    if (scrollInfo.nPos != scrollOffset)
    {
        int scrollAmount = roundToInt(scrollChar * (scrollOffset - scrollInfo.nPos));
        int scrollAmountHorizontal = 0;
        int scrollAmountVertical = 0;

        if (nBar == SB_VERT)
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

        HDC hdc = GetDC(hWindow);

        RECT windowRect;
        GetWindowRect(hWindow, &windowRect);

        renderer->ClearDC(hdc, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
        DeleteDC(hdc);
    }
}

SCROLLINFO ScrollProvider::GetScrollBarInfo(HWND hWindow, int nBar)
{
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(scrollInfo);
    scrollInfo.fMask = SIF_ALL;
    GetScrollInfo(hWindow, nBar, &scrollInfo);

    return scrollInfo;
}

ScrollProvider::~ScrollProvider()
{
}