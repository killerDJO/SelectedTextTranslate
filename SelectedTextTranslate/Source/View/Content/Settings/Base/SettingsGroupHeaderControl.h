#pragma once
#include "View\Framework\View\Views\ContentView.h"
#include "View\Content\Settings\Base\Enums\SettingsGroupVisibilityState.h"
#include "View\Content\Settings\Base\Enums\SettingsGroupContentState.h"

class SettingsGroupHeaderControl : public ContentView
{
private:
    int borderWidth;

    SettingsGroupVisibilityState visibilityState;
    SettingsGroupContentState contentState;
    wstring title;

protected:
    Size RenderContent(Renderer* renderer) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    SettingsGroupHeaderControl(CommonContext* context, View* parentView);

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void SetContentState(SettingsGroupContentState contentState);
    SettingsGroupContentState GetContentState() const;

    void SetVisibilityState(SettingsGroupVisibilityState visibilityState);
    SettingsGroupVisibilityState GetVisibilityState() const;

    void Initialize() override;

    Subscribeable<> OnSettingsToggled;
};
