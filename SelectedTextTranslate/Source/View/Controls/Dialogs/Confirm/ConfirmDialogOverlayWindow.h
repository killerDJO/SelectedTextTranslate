#pragma once
#include "View\Framework\View\Views\ChildView.h"

class ConfirmDialogOverlayWindow : public ChildView
{
private:
    Brush* backgroundBrush;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogOverlayWindow(ViewContext* context, View* parentWindow);
    ~ConfirmDialogOverlayWindow();

    void Initialize() override;
};