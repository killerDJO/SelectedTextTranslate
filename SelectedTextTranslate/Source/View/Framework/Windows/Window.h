#pragma once
#include "View\Framework\WindowContext.h"
#include "View\Framework\DeviceContextBuffer.h"
#include "View\Framework\Dto\WindowDescriptor.h"
#include "View\Framework\Enums\WindowStates.h"
#include "View\Framework\Windows\NativeWindowHolder.h"

class WindowContext;
class Renderer;

class Window : public NativeWindowHolder
{
private:
    vector<Window*> destroyBeforeDrawList;

    Size RenderToBuffer();

    void DrawChildWindows();
    void DestroyChildWindows(vector<Window*>& childWindows) const;

    void ApplyWindowPosition(bool preserveScrolls);

protected:
    WindowContext* context;
    WindowDescriptor descriptor;
    Size windowSize;
    Size contentSize;
    Point position;
    WindowStates windowState;
    bool isVisible;
    wstring name;

    DeviceContextBuffer* deviceContextBuffer;
    vector<Window*> activeChildWindows;

    DWORD GetScrollStyle() const;

    void AddChildWindow(Window* childWindow);
    void DestroyChildWindows();

    void ApplyRenderedState(bool preserveScrolls);
    virtual Size RenderContent(Renderer* renderer) = 0;
    virtual Point GetInitialWindowOffset();

    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    void AssertWindowInitialized() const;

public:
    Window(WindowContext* context, WindowDescriptor descriptor, wstring name);
    ~Window() override;

    WindowDescriptor GetDescriptor() const;

    Size GetSize() const;
    Size GetAvailableClientSize() const;
    Size GetContentSize() const;
    Point GetPosition() const;
    wstring GetName() const;

    void Show();
    void Hide();
    bool IsVisible() const;

    void Render(bool preserveScrolls = false);
    void Draw(bool drawChildren = false);

    void Initialize() override;
    virtual void Resize();
};