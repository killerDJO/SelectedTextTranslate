#pragma once
#include "Windows\Framework\Renderer.h"

class ScrollProvider
{
private:
    int scrollCharX;
    int scrollCharY;

    Renderer* renderer;

    void InitializeScrollbar(HWND hWindow, int windowDimension, int contentDimension, int scrollChar, int nBar);
    void ProcessScroll(HWND hWindow, WPARAM wParam, LPARAM lParam, int scrollChar, int nBar);
    void SetScrollPosition(HWND hWindow, SCROLLINFO scrollInfo, int nBar, int scrollOffset, int scrollChar);
    SCROLLINFO GetScrollBarInfo(HWND hWindow, int nBar);

public:
    ScrollProvider(Renderer* renderer);
    ~ScrollProvider();

    void InitializeScrollbars(HWND hWindow, int contentWidth, int contentHeight);
    void ProcessVerticalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam);
    void ProcessHorizontalScroll(HWND hWindow, WPARAM wParam, LPARAM lParam);

    int GetScrollPosition(HWND hWindow, int nBar);
    void SetScrollPosition(HWND hWindow, int nBar, int position);
};