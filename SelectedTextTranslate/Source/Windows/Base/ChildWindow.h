#pragma once
#include "Windows\Base\Window.h"

/// <summary>
/// This is a base class for any child window.
/// </summary>
/// <seealso cref="Window" />
class ChildWindow : public Window
{
protected:
    /// <summary>
    /// The parent window.
    /// </summary>
    Window* parentWindow;

    /// <summary>
    /// Specifies the window class.
    /// </summary>
    /// <param name="windowClass">The window class.</param>
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;

    // <summary>
    /// The Window Procedure.
    /// </summary>
    /// <param name="hWnd">The handle to window.</param>
    /// <param name="message">The message.</param>
    /// <param name="wParam">The wParam.</param>
    /// <param name="lParam">The lParam.</param>
    /// <returns>Handling result.</returns>
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="ChildWindow"/> class.
    /// </summary>
    /// <param name="context">The window context.</param>
    /// <param name="descriptor">The window descriptor.</param>
    /// <param name="parentWindow">The parent window.</param>
    ChildWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow);
    
    /// <summary>
    /// Finalizes an instance of the <see cref="ChildWindow"/> class.
    /// </summary>
    ~ChildWindow() override;

    /// <summary>
    /// Initializes this instance. Method is called during Initialize phase. This method creates native window.
    /// </summary>
    void Initialize() override;
};