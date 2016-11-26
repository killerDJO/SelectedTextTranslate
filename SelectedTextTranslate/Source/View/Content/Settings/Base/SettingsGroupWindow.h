#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Settings\Dto\Settings.h"
#include "View\Content\Settings\Base\Enums\SettingsGroupState.h"

class SettingsGroupWindow : public ContentWindow
{
protected:
    SettingsGroupState state;
    wstring title;
    Settings settings;

    HBRUSH backgroundBrush;

    Size RenderContent(Renderer* renderer) override;
    virtual void RenderSettingsContent(Renderer* renderer, Point contentPosition) = 0;

public:
    SettingsGroupWindow(WindowContext* context, Window* parentWindow);
    ~SettingsGroupWindow() override;

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void SetState(SettingsGroupState state);
    SettingsGroupState GetState() const;

    void Initialize() override;

    Subscribeable<> OnSettingsToggled;
};
