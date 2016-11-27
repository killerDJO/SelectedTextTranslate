#pragma once
#include "View\Framework\Windows\ChildWindow.h"

class ConfirmDialogOverlayWindow : public ChildWindow
{
protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogOverlayWindow(WindowContext* context, Window* parentWindow);
    ~ConfirmDialogOverlayWindow();

    void Initialize() override;
};