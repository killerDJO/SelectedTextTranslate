#pragma once
#include "Services\Settings\Dto\Settings.h"
#include "View\Framework\Rendering\Dto\RenderDescriptor.h"
#include "View\Framework\Windows\ContentWindow.h"
#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"

class SettingsGroupWindow : public ContentWindow
{
private:
    int borderWidth;

    void UpdateModificationState() const;

protected:
    SettingsGroupVisibilityState visibilityState;
    SettingsGroupContentState contentState;

    Settings currentSettings;
    Settings globalSettings;

    wstring title;

    SettingsGroupHeaderWindow* headerWindow;

    Size RenderContent(Renderer* renderer) override;
    virtual void RenderSettingsContent(RenderDescriptor renderDescriptor) = 0;

public:
    SettingsGroupWindow(WindowContext* context, Window* parentWindow);

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void SetSettings(Settings currentSettings, Settings globalSettings);

    SettingsGroupContentState GetContentState() const;

    void SetVisibilityState(SettingsGroupVisibilityState visibilityState);
    SettingsGroupVisibilityState GetVisibilityState() const;

    bool IsValid() const;
    virtual void ComputeContentState() = 0;

    Subscribeable<> OnSettingsToggled;
    Subscribeable<Settings> OnSettingsChanged;
};
