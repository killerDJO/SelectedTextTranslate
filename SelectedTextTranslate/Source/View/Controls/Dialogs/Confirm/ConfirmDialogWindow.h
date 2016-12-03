#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"

class ConfirmDialogWindow : public ChildWindow
{
private:
    wstring title;

    ConfirmDialogContentWindow* dialogContentWindow;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogWindow(WindowContext* context, Window* parentWindow);
    ~ConfirmDialogWindow() override;

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void Initialize() override;

    Subscribeable<> OnConfirm;
    Subscribeable<> OnCancel;
};
