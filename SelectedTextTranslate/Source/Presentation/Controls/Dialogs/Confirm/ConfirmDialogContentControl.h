#pragma once
#include "Presentation\Framework\Views\ControlView.h"

class ConfirmDialogContentControl : public ControlView
{
private:
    int paddingX;
    int paddingY;
    int lineHeight;
    int height;
    int borderWidth;

    Font* fontSmall;
    Font* fontSmallUnderscored;

    Brush* grayBrush;
    Brush* backgroundBrush;

    wstring title;

    Font* headerFont;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    ConfirmDialogContentControl(CommonContext* context, View* parentWindow);
    ~ConfirmDialogContentControl();

    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void Initialize() override;

    Subscribeable<> OnConfirm;
    Subscribeable<> OnCancel;
};
