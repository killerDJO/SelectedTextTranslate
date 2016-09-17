#pragma once
#include "Windows\Framework\RenderingContext.h"

class ScrollProvider
{
private:
    int scrollCharX;
    int scrollCharY;

    RenderingContext* renderingContext;

    void InitializeScrollbar(HWND hWindow, int windowDimension, int contentDimension, int scrollChar, int nBar);
    void ProcessScroll(HWND hWindow, WPARAM wParam, LPARAM lParam, int scrollChar, int nBar);
    void SetScrollPosition(HWND hWindow, SCROLLINFO scrollInfo, int nBar, int scrollOffset, int scrollChar);
    SCROLLINFO GetScrollBarInfo(HWND hWindow, int nBar);

public:
    ScrollProvider(RenderingContext* renderingContext);
    ~ScrollProvider();

    void InitializeScrollbar(HWND hWindow, int contentSize, int windowSize, int nBar);
    void ProcessVerticalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam);
    void ProcessHorizontalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam);

    int GetScrollPosition(HWND hWindow, int nBar);
    void SetScrollPosition(HWND hWindow, int nBar, int position);
};