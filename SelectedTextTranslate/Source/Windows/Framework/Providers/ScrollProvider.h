#pragma once
#include "Windows\Framework\Enums\ScrollBars.h"
#include "Windows\Base\Window.h"

class Window;

/// <summary>
/// Provides methods to create/process scroll.
/// </summary>
class ScrollProvider
{
private:
    /// <summary>
    /// Amount of pixels to scroll in horizontal direction.
    /// </summary>
    int scrollCharX;

    /// <summary>
    /// Amount of pixels to scroll in vertical direction.
    /// </summary>
    int scrollCharY;

    /// <summary>
    /// The vertical scroll bar width.
    /// </summary>
    int verticalScrollBarWidth;

    /// <summary>
    /// The horizontal scroll bar height.
    /// </summary>
    int horizontalScrollBarHeight;

    /// <summary>
    /// Initializes the scrollbar.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    /// <param name="contentDimension">The content dimension.</param>
    /// <param name="windowDimension">The window dimension.</param>
    /// <param name="scrollChar">The scroll amount.</param>
    /// <param name="scrollBar">The scroll bar.</param>
    void InitializeScrollbar(HWND windowHandle, int contentDimension, int windowDimension, int scrollChar, ScrollBars scrollBar) const;

    /// <summary>
    /// Processes the scroll WM_XSCROLL message. Scroll direction depends on scrollBar parameter.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="wParam">The wParam.</param>
    /// <param name="lParam">The lParam.</param>
    /// <param name="scrollChar">The scroll character.</param>
    /// <param name="scrollBar">The scroll bar.</param>
    void ProcessScroll(Window* window, WPARAM wParam, LPARAM lParam, int scrollChar, ScrollBars scrollBar) const;

    /// <summary>
    /// Sets the scroll position.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="scrollInfo">The scroll information.</param>
    /// <param name="scrollBar">The scroll bar.</param>
    /// <param name="scrollOffset">The scroll offset.</param>
    /// <param name="scrollChar">The scroll character.</param>
    void SetScrollPosition(Window* window, SCROLLINFO scrollInfo, ScrollBars scrollBar, int scrollOffset, int scrollChar) const;
    
    /// <summary>
    /// Gets the window scroll information.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="scrollBar">The scroll bar.</param>
    /// <returns>The scroll information.</returns>
    SCROLLINFO GetWindowScrollInfo(Window* window, ScrollBars scrollBar) const;

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="ScrollProvider"/> class.
    /// </summary>
    ScrollProvider();

    /// <summary>
    /// Finalizes an instance of the <see cref="ScrollProvider"/> class.
    /// </summary>
    ~ScrollProvider();

    /// <summary>
    /// Initializes the scrollbars.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="showHorizontalScroll">if set to <c>true</c> horizontal scroll bar will be initialized.</param>
    /// <param name="showVerticalScroll">if set to <c>true</c> vertical scroll bar will be initialized.</param>
    void InitializeScrollbars(Window* window, bool showHorizontalScroll, bool showVerticalScroll) const;

    /// <summary>
    /// Processes the WM_VSCROLL message.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="wParam">The wParam.</param>
    /// <param name="lParam">The lParam.</param>
    void ProcessVerticalScroll(Window* window, WPARAM wParam, LPARAM lParam) const;

    /// <summary>
    /// Processes the WM_HSCROLL message.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="wParam">The wParam.</param>
    /// <param name="lParam">The lParam.</param>
    void ProcessHorizontalScroll(Window* window, WPARAM wParam, LPARAM lParam) const;

    /// <summary>
    /// Gets the current scroll position of provided scroll bar.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="scrollBar">The scroll bar.</param>
    /// <returns>The current scroll position.</returns>
    int GetScrollPosition(Window* window, ScrollBars scrollBar) const;

    /// <summary>
    /// Sets the scroll position for provided scroll bar.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="scrollBar">The scroll bar.</param>
    /// <param name="position">The position.</param>
    void SetScrollPosition(Window* window, ScrollBars scrollBar, int position) const;

    /// <summary>
    /// Resets the scrolls position.
    /// </summary>
    /// <param name="window">The window.</param>
    void ResetScrollsPosition(Window* window) const;

    /// <summary>
    /// Gets the width of the vertical scroll bar.
    /// </summary>
    /// <returns></returns>
    int GetVerticalScrollBarWidth() const;

    /// <summary>
    /// Gets the height of the horizontal scroll bar.
    /// </summary>
    /// <returns></returns>
    int GetHorizontalScrollBarHeight() const;
};
