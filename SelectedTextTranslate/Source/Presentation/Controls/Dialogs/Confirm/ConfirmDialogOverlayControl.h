#pragma once
#include "Presentation\Framework\Views\ControlView.h"

class ConfirmDialogOverlayControl : public ControlView
{
private:
    Brush* backgroundBrush;

protected:
    void RenderContent(Renderer* renderer) override;
    void SpecifyWindow(NativeWindowHolder* window) override;

public:
    ConfirmDialogOverlayControl(ViewContext* context, View* parentWindow);
    ~ConfirmDialogOverlayControl();

    void SetSize(Size size);

    void Initialize() override;
};
