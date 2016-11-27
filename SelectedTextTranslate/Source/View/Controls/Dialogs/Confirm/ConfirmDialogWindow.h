#pragma once
#include "View\Content\Base\ContentWindow.h"

class ConfirmDialogWindow : public ChildWindow
{
protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogWindow(WindowContext* context, Window* parentWindow);
    ~ConfirmDialogWindow() override;

    void Initialize() override;
};