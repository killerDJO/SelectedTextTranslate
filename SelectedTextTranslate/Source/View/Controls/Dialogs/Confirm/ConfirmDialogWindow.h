#pragma once
#include "View\Framework\View\Views\ContentView.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"

class ConfirmDialogWindow : public ChildView
{
private:
    wstring title;

    ConfirmDialogContentWindow* dialogContentView;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogWindow(CommonContext* context, View* parentView);
    ~ConfirmDialogWindow() override;

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void Initialize() override;

    void Show() override;

    Subscribeable<> OnConfirm;
    Subscribeable<> OnCancel;
};
