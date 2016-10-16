#pragma once
#include "View\Framework\WindowContext.h"
#include "View\Framework\DeviceContextBuffer.h"
#include "View\Framework\Dto\WindowDescriptor.h"
#include "View\Framework\Enums\WindowStates.h"

class WindowContext;
class Renderer;

/// <summary>
/// Represents base class for any window. Provides basic functionality.
/// Window has several life-cycle phases: Initialize, Render, Draw, Destroy.
/// Initialize must be called first to initialize window.
/// Render must be called to change content of window and its dimensions/position.
/// Draw is called automatically after WM_PAIN message received. Also it can be forced by calling Draw method.
/// Destroy should be called when window is no longer required.
/// The following transitions are allowed:
/// Initialize -> Render -> (Render or Draw) -> ... -> (Render or Draw) -> Destroy (delete).
/// Render has two phases:
/// 1. Content is rendered, dimensions/position changed and child windows created / rendered.
/// 2. Window moved, scroll initialized, child window and window itself drawn.
/// </summary>
class Window
{
private:
    vector<Window*> destroyBeforeDrawList;

    Size RenderToBuffer();

    void DrawChildWindows();
    void DestroyChildWindows(vector<Window*>& childWindows) const;

    void ApplyWindowPosition(bool preserveScrolls);

protected:
    HWND windowHandle;
    wchar_t* className;

    WindowDescriptor descriptor;
    Size windowSize;
    Size contentSize;
    Point position;

    WindowStates windowState;
    bool isVisible;

    DeviceContextBuffer* deviceContextBuffer;

    WindowContext* context;
    vector<Window*> activeChildWindows;

    void AddChildWindow(Window* childWindow);
    void DestroyChildWindows();

    DWORD GetScrollStyle() const;

    virtual void SpecifyWindowClass(WNDCLASSEX* windowClass);
    virtual Size RenderContent(Renderer* renderer) = 0;

    void ApplyRenderedState(bool preserveScrolls);
    virtual Point GetInitialWindowOffset();

    static LRESULT CALLBACK WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam);

public:
    Window(WindowContext* context, WindowDescriptor descriptor);
    virtual ~Window();

    HWND GetHandle() const;
    WindowDescriptor GetDescriptor() const;

    Size GetSize() const;
    Size GetAvailableClientSize() const;
    Size GetContentSize() const;

    Point GetPosition() const;

    void Show();
    void Hide();
    bool IsVisible() const;

    void Render(bool preserveVerticalScroll = false);
    void Draw(bool drawChildren = false);

    virtual void Initialize();
    virtual void Resize();
};