#pragma once
#include "Presentation\Framework\Views\ChildView.h"

class ConfirmDialogOverlayWindow : public ChildView
{
private:
    Brush* backgroundBrush;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogOverlayWindow(CommonContext* context, View* parentWindow);
    ~ConfirmDialogOverlayWindow();

    void SetSize(Size size);

    void Initialize() override;
};