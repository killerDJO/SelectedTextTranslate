#pragma once
#include "View\Framework\View\Views\ContentView.h"

class ConfirmDialogContentWindow : public ContentView
{
private:
    int height;
    int borderWidth;

    wstring title;

    Font* headerFont;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogContentWindow(CommonContext* context, View* parentWindow);
    ~ConfirmDialogContentWindow();

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void Initialize() override;

    Subscribeable<> OnConfirm;
    Subscribeable<> OnCancel;
};
