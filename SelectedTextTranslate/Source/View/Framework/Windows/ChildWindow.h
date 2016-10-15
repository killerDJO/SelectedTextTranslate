#pragma once
#include "View\Framework\Windows\Window.h"

class ChildWindow : public Window
{
protected:
    Window* parentWindow;

    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Point GetInitialWindowOffset() override;
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    ChildWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow);
    ~ChildWindow() override;

    void Initialize() override;
};