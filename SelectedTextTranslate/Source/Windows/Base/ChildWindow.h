#pragma once
#include "Windows\Base\Window.h"
#include "Windows\Framework\Renderer.h"

class ChildWindow : public Window
{
protected:
    HWND parentWindow;
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    ChildWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow);
    ~ChildWindow() override;

    void Initialize() override;
};