#pragma once
#include "View\Framework\Windows\Window.h"

class ChildWindow : public Window
{
protected:
    Window* parentWindow;

    Point GetInitialWindowOffset() override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    ChildWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow);
    ~ChildWindow() override;

    void Initialize() override;
};