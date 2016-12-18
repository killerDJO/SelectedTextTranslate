#pragma once
#include "View\Framework\Windows\ContentWindow.h"
#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"
#include "View\Framework\Rendering\Dto\RenderDescriptor.h"

class SettingsGroupWindow : public ContentWindow
{
private:
    void UpdateModificationState() const;

protected:
    SettingsGroupVisibilityState visibilityState;
    SettingsGroupContentState contentState;

    wstring title;

    SettingsGroupHeaderWindow* headerWindow;

    Size RenderContent(Renderer* renderer) override;
    virtual void RenderSettingsContent(RenderDescriptor renderDescriptor) = 0;

public:
    SettingsGroupWindow(WindowContext* context, Window* parentWindow);
    ~SettingsGroupWindow() override;

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    SettingsGroupContentState GetContentState() const;

    void SetVisibilityState(SettingsGroupVisibilityState visibilityState);
    SettingsGroupVisibilityState GetVisibilityState() const;

    void Initialize() override;

    bool IsValid() const;
    virtual void ComputeContentState() = 0;

    Subscribeable<> OnSettingsToggled;
};
