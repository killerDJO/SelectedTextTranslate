#pragma once
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogContentControl.h"
#include "Presentation\Framework\Views\ControlView.h"

class ConfirmDialogControl : public ControlView
{
private:
    wstring title;

    ConfirmDialogContentControl* dialogContentView;

protected:
    void RenderContent(Renderer* renderer) override;
    void SpecifyWindow(NativeWindowHolder* window) override;

public:
    ConfirmDialogControl(ViewContext* context, View* parentView);
    ~ConfirmDialogControl() override;

    void SetSize(Size size);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void Initialize() override;

    void Show();
    void Hide();

    Subscribable<> OnConfirm;
    Subscribable<> OnCancel;
};
