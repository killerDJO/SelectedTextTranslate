#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "View\Content\Settings\Base\Enums\SettingsGroupState.h"
#include "View\Content\Settings\Base\SettingsGroupHeaderWindow.h"
#include "View\Framework\Dto\Rendering\RenderDescriptor.h"

class SettingsGroupWindow : public ContentWindow
{
private:
    void UpdateModificationState() const;
    wstring GetCurrentTitle() const;

protected:
    SettingsGroupState state;
    wstring title;
    bool isModified;

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

    void SetState(SettingsGroupState state);
    SettingsGroupState GetState() const;

    void SetModifiedState();
    void SetNotModifiedState();
    bool IsModified() const;

    void Initialize() override;

    Subscribeable<> OnSettingsToggled;
};
