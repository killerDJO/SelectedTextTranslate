#pragma once
#include "View\Framework\Enums\ScrollBars.h"
#include "View\Framework\Windows\Window.h"
#include "View\Framework\Dto\Positioning\Size.h"

class Window;

class ScrollProvider
{
private:
    int scrollCharX;
    int scrollCharY;

    int verticalScrollBarWidth;
    int horizontalScrollBarHeight;

    void InitializeScrollbar(HWND windowHandle, int contentDimension, int windowDimension, ScrollBars scrollBar, int initialPosition) const;

    SCROLLINFO GetWindowScrollInfo(Window* window, ScrollBars scrollBar) const;
    void SetScrollPosition(Window* window, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset) const;
    void ProcessScroll(Window* window, WPARAM wParam, LPARAM lParam, ScrollBars scrollBar) const;

    Size AlignWithScrollingGrid(Size size) const;

public:
    ScrollProvider();

    void InitializeScrollbars(
        Window* window,
        bool showHorizontalScroll,
        bool showVerticalScroll,
        int initialVerticalScrollPosition = 0,
        int initialHorizontalScrollPostion = 0) const;
    void HideScrollbars(Window* window) const;

    void ProcessScrollMessages(Window* window, UINT message, WPARAM wParam, LPARAM lParam) const;

    int GetCurrentScrollPostion(Window* window, ScrollBars scrollBar) const;
    int GetCurrentScrollOffset(Window* window, ScrollBars scrollBar) const;

    bool IsScrollBarVisible(const Window* window, ScrollBars scrollBar) const;
    int GetScrollBarSize(const Window* window, ScrollBars scrollBar) const;

    int GetScrollChar(ScrollBars scrollBar) const;
};
