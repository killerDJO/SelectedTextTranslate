#pragma once
#include "View\Framework\WindowContext.h"
#include "View\Framework\Dto\WindowDescriptor.h"
#include "View\Framework\Enums\WindowStates.h"
#include "View\Framework\Windows\NativeWindowHolder.h"
#include "View\Framework\Rendering\DeviceContextBuffer.h"
#include "View\Framework\Rendering\Renderer.h"

class WindowContext;
class Renderer;

class Window : public NativeWindowHolder
{
private:
    vector<Window*> destroyBeforeDrawList;

    void DrawChildWindows();
    void DestroyChildWindows(vector<Window*>& childWindows) const;

protected:
    WindowContext* context;
    WindowDescriptor descriptor;
    Size windowSize;
    Size contentSize;
    Point position;
    WindowStates windowState;
    bool isVisible;

    DeviceContextBuffer* deviceContextBuffer;
    vector<Window*> activeChildWindows;

    DWORD GetScrollStyle() const;

    void AddChildWindow(Window* childWindow);
    void DestroyChildWindows();

    void ApplyRenderedState(bool preserveScrolls);
    void ApplyWindowPosition(bool preserveScrolls);
    Size RenderToBuffer();
    virtual Size RenderContent(Renderer* renderer) = 0;
    virtual Point GetInitialWindowOffset();

    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    void AssertWindowInitialized() const;
    void AssertWindowNotInitialized() const;

public:
    Window(WindowContext* context);
    ~Window() override;

    WindowDescriptor GetDescriptor() const;
    virtual void SetDescriptor(WindowDescriptor descriptor);

    Size GetSize(bool downscale = true) const;
    Size GetAvailableClientSize(bool downscale = true) const;
    Size GetContentSize() const;
    Point GetPosition(bool downscale = true) const;
    Rect GetBoundingRect(bool downscale = true) const;

    void MakeVisible();
    void MakeHidden();
    bool IsVisible() const;

    virtual void Show();
    virtual void Hide();

    void Render(bool preserveScrolls = false);
    void Draw(bool drawChildren = false);

    void Initialize() override;
    virtual void Resize();
};