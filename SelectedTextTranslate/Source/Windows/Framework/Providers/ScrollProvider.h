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

    void InitializeScrollbar(HWND windowHandle, int contentDimension, int windowDimension, int scrollChar, ScrollBars scrollBar) const;
    void ProcessScroll(Window* window, WPARAM wParam, LPARAM lParam, int scrollChar, ScrollBars scrollBar) const;
    void SetScrollPosition(Window* window, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset, int scrollChar) const;
    SCROLLINFO GetWindowScrollInfo(Window* window, ScrollBars scrollBar) const;

public:
    ScrollProvider();
    ~ScrollProvider();

    void InitializeScrollbars(Window* window, bool showHorizontalScroll, bool showVerticalScroll) const;
    void ProcessVerticalScroll(Window* window, WPARAM wParam, LPARAM lParam) const;
    void ProcessHorizontalScroll(Window* window, WPARAM wParam, LPARAM lParam) const;

    int GetScrollPosition(Window* window, ScrollBars scrollBar) const;
    void SetScrollPosition(Window* window, ScrollBars scrollBar, int position) const;
    void ResetScrollsPosition(Window* window) const;

    int GetVerticalScrollBarWidth() const;
    int GetHorizontalScrollBarHeight() const;
};
