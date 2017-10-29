#pragma once
#include "Presentation\Components\Settings\Base\Enums\SettingsGroupVisibilityState.h"
#include "Presentation\Components\Settings\Base\Enums\SettingsGroupContentState.h"
#include "Presentation\Framework\Views\ControlView.h"

class SettingsGroupHeaderControl : public ControlView
{
private:
    int borderWidth;

    int paddingX;
    int paddingY;
    int lineHeight;
    Brush* backgroundBrush;
    Font* fontNormal;

    SettingsGroupVisibilityState visibilityState;
    SettingsGroupContentState contentState;
    wstring title;

protected:
    Size RenderContent(Renderer* renderer) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    SettingsGroupHeaderControl(ViewContext* context, View* parentView);
    ~SettingsGroupHeaderControl();

    SettingsGroupHeaderControl* SetDimensions(Point position, int width);

    SettingsGroupHeaderControl* SetTitle(wstring title);
    wstring GetTitle() const;

    SettingsGroupHeaderControl* SetContentState(SettingsGroupContentState contentState);
    SettingsGroupContentState GetContentState() const;

    SettingsGroupHeaderControl* SetVisibilityState(SettingsGroupVisibilityState visibilityState);
    SettingsGroupVisibilityState GetVisibilityState() const;

    void Initialize() override;

    Subscribable<> OnSettingsToggled;
};
