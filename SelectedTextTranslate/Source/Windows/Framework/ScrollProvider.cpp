#include "Windows\Framework\ScrollProvider.h"
#include "RenderingContext.h"

ScrollProvider::ScrollProvider(RenderingContext* renderingContext)
{
    this->renderingContext = renderingContext;
    this->scrollCharX = 8;
    this->scrollCharY = 20;
}

void ScrollProvider::InitializeScrollbar(Window* window, int contentSize, int windowSize, ScrollBars scrollBar)
{
    int scrollChar = scrollBar == ScrollBars::Vertical
        ? scrollCharY
        : scrollCharX;

    InitializeScrollbar(window, windowSize, contentSize, scrollChar, scrollBar);
}

void ScrollProvider::InitializeScrollbar(Window* window, int windowDimension, int contentDimension, int scrollChar, ScrollBars scrollBar) const
{
    SCROLLINFO si;

    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nPos = 0;
    si.nMax = roundToInt(contentDimension / scrollChar);
    si.nPage = roundToInt(windowDimension / scrollChar);
    SetScrollInfo(window->GetHandle(), (int)scrollBar, &si, TRUE);
}

int ScrollProvider::GetScrollPosition(Window* window, ScrollBars scrollBar) const
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(window, scrollBar);
    return scrollInfo.nPos;
}

void ScrollProvider::SetScrollPosition(Window* window, ScrollBars scrollBar, int position) const
{
    SCROLLINFO scrollInfo = GetScrollBarInfo(window, scrollBar);
    int scrollOffset = scrollInfo.nPos;

    scrollInfo.nPos = min(scrollInfo.nMax - scrollInfo.nPage + 1, (unsigned)max(position, 0));

    SetScrollPosition(window, scrollInfo, scrollBar, scrollOffset, scrollCharY);
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
    SCROLLINFO scrollInfo = GetScrollBarInfo(window, scrollBar);
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

        window->ForceDraw();
       /* HDC hdc = GetDC(window->GetHandle());

        renderingContext->ClearDC(hdc, window->GetWidth(), window->GetHeight());

        DeleteDC(hdc);*/
    }
}

SCROLLINFO ScrollProvider::GetScrollBarInfo(Window* window, ScrollBars scrollBar) const
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