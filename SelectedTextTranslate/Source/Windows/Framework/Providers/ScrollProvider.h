#pragma once
#include "Windows\Framework\Enums\ScrollBars.h"
#include "Windows\Base\Window.h"

class Window;

class ScrollProvider
{
private:
    int scrollCharX;
    int scrollCharY;

    void InitializeScrollbar(Window* window, int contentDimension, int scrollChar, ScrollBars scrollBar) const;
    void ProcessScroll(Window* window, WPARAM wParam, LPARAM lParam, int scrollChar, ScrollBars scrollBar) const;
    void SetScrollPosition(Window* window, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset, int scrollChar) const;
    SCROLLINFO GetScrollBarInfo(Window* window, ScrollBars scrollBar) const;

public:
    ScrollProvider();
    ~ScrollProvider();

    void InitializeScrollbar(Window* window, int contentSize, ScrollBars scrollBar);
    void ProcessVerticalScroll(Window* window, WPARAM wParam, LPARAM lParam) const;
    void ProcessHorizontalScroll(Window* window, WPARAM wParam, LPARAM lParam) const;

    int GetScrollPosition(Window* window, ScrollBars scrollBar) const;
    void SetScrollPosition(Window* window, ScrollBars scrollBar, int position) const;

    void ResetScrollPosition(Window* window);
};
