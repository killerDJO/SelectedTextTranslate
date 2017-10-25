#pragma once
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogContentControl.h"
#include "Presentation\Framework\Views\ControlView.h"

class ConfirmDialogControl : public ControlView
{
private:
    wstring title;

    ConfirmDialogContentControl* dialogContentView;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogControl(CommonContext* context, View* parentView);
    ~ConfirmDialogControl() override;

    void SetSize(Size size);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void Initialize() override;

    void Show() override;

    Subscribeable<> OnConfirm;
    Subscribeable<> OnCancel;
};
