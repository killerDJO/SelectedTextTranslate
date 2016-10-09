#pragma once
#include "Windows\Framework\WindowContext.h"
#include "Windows\Framework\DeviceContextBuffer.h"
#include "Windows\Framework\Dto\WindowDescriptor.h"
#include "Windows\Framework\Enums\WindowStates.h"

class WindowContext;
class Renderer;

/// <summary>
/// Represents base class for any window. Provides basic functionality.
/// Window has several life-cycle phases: Initialize, Render, Draw, Destroy.
/// Initialize must be called first to initialize window.
/// Render must be called to change content of window and its dimensions/position.
/// Draw is called automatically after WM_PAIN message received. Also it can be forced by calling ForceDraw method.
/// Destroy should be called when window is no longer necessary.
/// The following transitions are allowed:
/// Initialize -> Render -> (Render or Draw) -> ... -> (Render or Draw) -> Destroy (delete).
/// </summary>
class Window
{
private:
    /// <summary>
    /// The current window state.
    /// </summary>
    WindowStates windowState;

    /// <summary>
    /// List of windows to destroy on the next Draw phase.
    /// It may hold windows which has been scheduled to destroy on Render phase. It can have data only between Render and Draw states.
    /// It is necessary to have such list to avoid preliminary destroying window on render before actual draw occurs.
    /// </summary>
    vector<Window*> destroyBeforeDrawList;

    /// <summary>
    /// Utility method which is used to render window content to the buffering device context.
    /// It may increase the buffer size if necessary.
    /// </summary>
    /// <returns>The size of rendered content.</returns>
    Size RenderToBuffer();

    /// <summary>
    /// Draws buffered device context onto the screen's device context. Used after WM_PAINT requested.
    /// </summary>
    void Draw();

    /// <summary>
    /// Draws the child windows.
    /// </summary>
    void DrawChildWindows();

    /// <summary>
    /// Destroys the provided child windows.
    /// </summary>
    /// <param name="childWindows">The child windows to destroy.</param>
    void DestroyChildWindows(vector<Window*>& childWindows) const;

    /// <summary>
    /// Gets the size of the provided scroll bar. Used to compute available window client size.
    /// </summary>
    /// <param name="scrollBar">The scroll bar.</param>
    /// <returns></returns>
    int GetScrollBarSize(ScrollBars scrollBar) const;

protected:
    /// <summary>
    /// The window handle.
    /// </summary>
    HWND windowHandle;

    /// <summary>
    /// The name of window class.
    /// </summary>
    wchar_t* className;

    /// <summary>
    /// The window descriptor which holds window initial size and position, overflow and scaling info.
    /// </summary>
    WindowDescriptor descriptor;

    /// <summary>
    /// The current window size.
    /// </summary>
    Size windowSize;

    /// <summary>
    /// The current content size.
    /// </summary>
    Size contentSize;

    /// <summary>
    /// The current window position relative to parent (for root window it's his position relative to screen).
    /// </summary>
    Point position;

    /// <summary>
    /// Shows whether or not current window visible. 
    /// The actual state of the native window will be changed during draw phase.
    /// </summary>
    bool isVisible;

    /// <summary>
    /// The device context buffer which holds rendered window state.
    /// It's used to avoid unnecessary re-rendering during scroll, resize or move.
    /// </summary>
    DeviceContextBuffer* deviceContextBuffer;

    /// <summary>
    /// The context which holds different providers and other contexts.
    /// </summary>
    WindowContext* context;

    /// <summary>
    /// List of actual child windows.
    /// It may hold not yet drawn window between Render and Draw states.
    /// </summary>
    vector<Window*> activeChildWindows;

    /// <summary>
    /// Adds the child window to the list of child windows. Window will be automatically initialized and drawn after draw is requested.
    /// </summary>
    /// <param name="childWindow">The child window.</param>
    void AddChildWindow(Window* childWindow);

    /// <summary>
    /// Destroys the current active child windows.
    /// </summary>
    void DestroyChildWindows();

    /// <summary>
    /// Gets the scroll styles to create window.
    /// </summary>
    /// <returns></returns>
    DWORD GetScrollStyle() const;

    /// <summary>
    /// Specifies the window class structure. Called during Initialize phase.
    /// Must be implemented by child class in order to customize window appearance and behavior. Also is using to set custom WndProc.
    /// </summary>
    /// <param name="windowClass">The WNDCLASSEX structure which holds base data.</param>
    virtual void SpecifyWindowClass(WNDCLASSEX* windowClass) = 0;

    /// <summary>
    /// This method should render the window content using provided renderer. It also should create any child window if necessary.
    /// Must be implemented by child class. Called during Render.
    /// </summary>
    /// <param name="renderer">The renderer.</param>
    /// <returns></returns>
    virtual Size RenderContent(Renderer* renderer) = 0;

    /// <summary>
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
    /// Initializes a new instance of the <see cref="Window"/> class.
    /// </summary>
    /// <param name="context">The window context.</param>
    /// <param name="descriptor">The window descriptor.</param>
    Window(WindowContext* context, WindowDescriptor descriptor);
    
    /// <summary>
    /// Finalizes an instance of the <see cref="Window"/> class.
    /// </summary>
    virtual ~Window();

    /// <summary>
    /// Gets the handle of the current window.
    /// </summary>
    /// <returns>The handle of the current window.</returns>
    HWND GetHandle() const;

    /// <summary>
    /// Gets the size of the current window.
    /// </summary>
    /// <returns>The size of the current window.</returns>
    Size GetSize() const;

    /// <summary>
    /// Gets the client size of the window including areas occupied by scroll.
    /// </summary>
    /// <returns>The available client size.</returns>
    Size GetAvailableClientSize() const;

    /// <summary>
    /// Gets the client size of the window excluding occupied by scroll areas.
    /// </summary>
    /// <returns>The current client size.</returns>
    Size GetCurrentClientSize() const;

    /// <summary>
    /// Gets the size of the rendered content.
    /// </summary>
    /// <returns>The size of the rendered content.</returns>
    Size GetContentSize() const;

    /// <summary>
    /// Gets the current window position.
    /// </summary>
    /// <returns>The current window position.</returns>
    Point GetPosition() const;

    /// <summary>
    /// Shows this window.
    /// </summary>
    void Show();

    /// <summary>
    /// Hides this window.
    /// </summary>
    void Hide();

    /// <summary>
    /// Determines whether this window is visible.
    /// May not match the actual state of the window if called between Render and Draw states.
    /// </summary>
    /// <returns>
    ///   <c>true</c> if this window is visible; otherwise, <c>false</c>.
    /// </returns>
    bool IsVisible() const;

    /// <summary>
    /// Renders the window content onto buffer, resize window according to descriptor and requests Draw.
    /// </summary>
    /// <param name="preserveVerticalScroll">if set to <c>true</c> current vertical scroll position will be preserved.</param>
    void Render(bool preserveVerticalScroll = false);

    /// <summary>
    /// Forces the draw. This method draws content immediately without sending the WM_DRAW message.
    /// </summary>
    void ForceDraw();

    /// <summary>
    /// Initializes this instance. This method creates native window.
    /// </summary>
    virtual void Initialize();

    /// <summary>
    /// Resizes this instance. Child windows may override this method to provide custom resizing behavior.
    /// </summary>
    virtual void Resize();
};