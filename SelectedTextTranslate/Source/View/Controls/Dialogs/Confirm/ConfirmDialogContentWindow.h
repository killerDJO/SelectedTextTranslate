#pragma once
#include "View\Framework\Windows\ContentWindow.h"

class ConfirmDialogContentWindow : public ContentWindow
{
private:
    wstring title;

    HFONT headerFont;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogContentWindow(WindowContext* context, Window* parentWindow);
    ~ConfirmDialogContentWindow();

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void Initialize() override;

    Subscribeable<> OnConfirm;
    Subscribeable<> OnCancel;
};
