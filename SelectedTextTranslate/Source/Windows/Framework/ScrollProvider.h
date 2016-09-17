#pragma once
#include "Windows\Framework\RenderingContext.h"
#include "Windows\Framework\Enums\ScrollBars.h"

class ScrollProvider
{
private:
    int scrollCharX;
    int scrollCharY;

    void InitializeScrollbar(HWND hWindow, int windowDimension, int contentDimension, int scrollChar, ScrollBars scrollBar);
    void ProcessScroll(HWND hWindow, WPARAM wParam, LPARAM lParam, int scrollChar, ScrollBars scrollBar);
    void SetScrollPosition(HWND hWindow, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset, int scrollChar);
    SCROLLINFO GetScrollBarInfo(HWND hWindow, ScrollBars scrollBar);

public:
    ScrollProvider();
    ~ScrollProvider();

    void InitializeScrollbar(HWND hWindow, int contentSize, int windowSize, ScrollBars scrollBar);
    void ProcessVerticalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam);
    void ProcessHorizontalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam);

    int GetScrollPosition(HWND hWindow, ScrollBars scrollBar);
    void SetScrollPosition(HWND hWindow, ScrollBars scrollBar, int position);
};