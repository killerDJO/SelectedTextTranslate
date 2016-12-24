#pragma once
#include "View\Framework\Windows\Window.h"

class ChildWindow : public Window
{
protected:
    Window* parentWindow;
    bool isLayered;

    Point GetInitialWindowOffset() override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    ChildWindow(WindowContext* context, Window* parentWindow);

    void Initialize() override;

    void EnableLayeredMode();
};