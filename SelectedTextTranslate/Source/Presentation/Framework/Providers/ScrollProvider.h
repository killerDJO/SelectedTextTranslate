#pragma once
#include "Presentation\Framework\Enums\ScrollBars.h"
#include "Presentation\Framework\Views\View.h"

class View;

class ScrollProvider
{
private:
    int scrollCharX;
    int scrollCharY;

    int verticalScrollBarWidth;
    int horizontalScrollBarHeight;

    void InitializeScrollbar(HWND windowHandle, int contentDimension, int windowDimension, ScrollBars scrollBar, int initialPosition) const;

    SCROLLINFO GetWindowScrollInfo(View* window, ScrollBars scrollBar) const;
    void SetScrollPosition(View* window, SCROLLINFO scrollInfo, ScrollBars scrollBar, int currentScrollPosition) const;
    void ProcessScroll(View* window, WPARAM wParam, LPARAM lParam, ScrollBars scrollBar) const;
    int ComputeScrollAdjustment(HWND windowHandle, SCROLLINFO scrollInfo, ScrollBars scrollBar, int currentScrollPosition) const;

public:
    ScrollProvider();

    void InitializeScrollbars(
        View* window,
        bool showHorizontalScroll,
        bool showVerticalScroll,
        int initialVerticalScrollPosition = 0,
        int initialHorizontalScrollPostion = 0) const;
    void HideScrollbars(View* window) const;

    LRESULT ProcessScrollMessages(View* window, UINT message, WPARAM wParam, LPARAM lParam) const;

    int GetCurrentScrollPostion(View* window, ScrollBars scrollBar) const;
    int GetCurrentScrollOffset(View* window, ScrollBars scrollBar) const;

    bool IsScrollBarVisible(const View* window, ScrollBars scrollBar) const;
    int GetScrollBarSize(const View* window, ScrollBars scrollBar) const;

    int GetScrollChar(ScrollBars scrollBar) const;
};
