#pragma once
#include "Windows\Framework\Enums\ScrollBars.h"
#include "Windows\Base\Window.h"

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
    int GetScrollChar(ScrollBars scrollBar) const;

public:
    ScrollProvider();
    ~ScrollProvider();

    void InitializeScrollbars(
        Window* window,
        bool showHorizontalScroll,
        bool showVerticalScroll,
        int initialVerticalScrollPosition = 0,
        int initialHorizontalScrollPostion = 0) const;
    void ProcessScroll(Window* window, WPARAM wParam, LPARAM lParam, ScrollBars scrollBar) const;

    int GetCurrentScrollPostion(Window* window, ScrollBars scrollBar) const;
    int GetCurrentScrollOffset(Window* window, ScrollBars scrollBar) const;

    void ResetScrollsPosition(Window* window) const;

    bool IsScrollBarVisible(const Window* window, ScrollBars scrollBar) const;
    int GetScrollBarSize(const Window* window, ScrollBars scrollBar) const;
};
