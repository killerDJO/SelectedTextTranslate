#pragma once
#include "Windows\Base\ChildWindow.h"

/// <summary>
/// The common base class for buttons which change state on hover.
/// </summary>
/// <seealso cref="ChildWindow" />
class HoverButtonWindow : public ChildWindow
{
private:
    /// <summary>
    /// The callback to execute on click.
    /// </summary>
    function<void()> clickCallback;

    /// <summary>
    /// Indicates whether or not button is hovered.
    /// </summary>
    bool isHovered;

    /// <summary>
    /// The Window Procedure.
    /// </summary>
    /// <param name="hWnd">The handle to window.</param>
    /// <param name="message">The message.</param>
    /// <param name="wParam">The wParam.</param>
    /// <param name="lParam">The lParam.</param>
    /// <returns>Handling result.</returns>
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    /// <summary>
    /// The device context which holds rendered hover state.
    /// </summary>
    HDC hoverStateDeviceContext;

    /// <summary>
    /// The device context which holds rendered normal state.
    /// </summary>
    HDC normalStateDeviceContext;

    /// <summary>
    /// Renders the current button state.
    /// </summary>
    /// <param name="renderer">The renderer.</param>
    /// <returns>The size of rendered content.</returns>
    Size RenderContent(Renderer* renderer) override;

    /// <summary>
    /// Renders the normal and hover states device contexts.
    /// </summary>
    virtual void RenderStatesDeviceContext() = 0;

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="HoverButtonWindow"/> class.
    /// </summary>
    /// <param name="context">The window context.</param>
    /// <param name="descriptor">The window descriptor.</param>
    /// <param name="parentWindow">The parent window.</param>
    /// <param name="clickCallback">The click callback.</param>
    HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, function<void()> clickCallback);
   
    /// <summary>
    /// Finalizes an instance of the <see cref="HoverButtonWindow"/> class.
    /// </summary>
    virtual ~HoverButtonWindow();

    /// <summary>
    /// Initializes this hover button window.
    /// </summary>
    void Initialize() override;
};