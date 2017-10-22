#pragma once
#include "Presentation\Framework\Views\ContentView.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogContentWindow.h"

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

    void SetSize(Size size);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void Initialize() override;

    void Show() override;

    Subscribeable<> OnConfirm;
    Subscribeable<> OnCancel;
};
