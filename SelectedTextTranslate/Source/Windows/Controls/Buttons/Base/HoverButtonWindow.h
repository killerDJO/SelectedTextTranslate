#pragma once
#include "Windows\Base\ChildWindow.h"

class HoverButtonWindow : public ChildWindow
{
private:
    function<void()> clickCallback;

    bool isHovered;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    HDC hoverStateDeviceContext;
    HDC normalStateDeviceContext;

    Size RenderContent(Renderer* renderer) override;

    virtual void RenderStatesDeviceContext() = 0;

public:
    HoverButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, function<void()> clickCallback);
    virtual ~HoverButtonWindow();

    void Initialize() override;
};