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
    vector<Window*> destroyBeforeDrawList;

    Size RenderToBuffer();
    void ApplyRenderedChanges();

    void DrawChildWindows();
    void DestroyChildWindows(vector<Window*>& childWindows) const;

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

    virtual void SpecifyWindowClass(WNDCLASSEX* windowClass) = 0;
    virtual Size RenderContent(Renderer* renderer) = 0;
    virtual Point GetInitialWindowOffset();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    Window(WindowContext* context, WindowDescriptor descriptor);
    virtual ~Window();

    HWND GetHandle() const;

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