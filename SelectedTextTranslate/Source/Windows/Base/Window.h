#pragma once
#include "Windows\Framework\WindowContext.h"
#include "Windows\Framework\Dto\WindowDescriptor.h"

class WindowContext;
class Renderer;

class Window
{
private:
    vector<Window*> activeChildWindows;
    vector<Window*> destroyBeforeDrawQueue;

    void Draw();
    void DrawChildWindows();
    void DestroyChildWindows(vector<Window*>& childWindows) const;

protected:
    HWND hWindow;
    wchar_t* className;

    WindowDescriptor descriptor;
    Size windowSize;
    Size contentSize;

    HDC inMemoryDC;
    Size dcSize;

    WindowContext* context;

    virtual void SpecifyWindowClass(WNDCLASSEX* windowClass) = 0;

    virtual Size RenderContent();
    virtual Size RenderDC(Renderer* renderer) = 0;

    void AddChildWindow(Window* childWindow);

    DWORD GetScrollStyle() const;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    Window(WindowContext* context, WindowDescriptor descriptor);
    virtual ~Window();

    HWND GetHandle() const;
    int GetWidth() const;
    int GetHeight() const;

    virtual void Initialize();
    void Show() const;
    void Hide() const;

    void Render(bool preserveVerticalScroll = false);
    void ForceDraw() const;
};