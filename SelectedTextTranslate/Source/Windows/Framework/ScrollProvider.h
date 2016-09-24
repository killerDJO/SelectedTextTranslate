#pragma once
#include "Windows\Framework\Enums\ScrollBars.h"

class ScrollProvider
{
private:
    int scrollCharX;
    int scrollCharY;

    void InitializeScrollbar(HWND hWindow, int windowDimension, int contentDimension, int scrollChar, ScrollBars scrollBar) const;
    void ProcessScroll(HWND hWindow, WPARAM wParam, LPARAM lParam, int scrollChar, ScrollBars scrollBar) const;
    void SetScrollPosition(HWND hWindow, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset, int scrollChar) const;
    SCROLLINFO GetScrollBarInfo(HWND hWindow, ScrollBars scrollBar) const;

public:
    ScrollProvider();
    ~ScrollProvider();

    void InitializeScrollbar(HWND hWindow, int contentSize, int windowSize, ScrollBars scrollBar);
    void ProcessVerticalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam) const;
    void ProcessHorizontalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam) const;

    int GetScrollPosition(HWND hWindow, ScrollBars scrollBar) const;
    void SetScrollPosition(HWND hWindow, ScrollBars scrollBar, int position) const;
};