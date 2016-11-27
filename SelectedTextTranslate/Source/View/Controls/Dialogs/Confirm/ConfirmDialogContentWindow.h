#pragma once
#include "View\Content\Base\ContentWindow.h"

class ConfirmDialogContentWindow : public ContentWindow
{
private:
    wstring text;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogContentWindow(WindowContext* context, Window* parentWindow);
    ~ConfirmDialogContentWindow();

    void Initialize() override;

    void SetText(wstring text);
    wstring GetText() const;

    Subscribeable<> OnConfirm;
    Subscribeable<> OnCancel;
};
