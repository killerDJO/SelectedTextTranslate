#pragma once
#include "View\Content\Base\ContentWindow.h"

class SettingGroupWindow : public ContentWindow
{
private:
    bool isCollapsed;
    wstring title;

protected:
    Size RenderContent(Renderer* renderer) override;
    virtual void RenderSettingsContent(Renderer* renderer, Point contentPosition) = 0;

public:
    SettingGroupWindow(WindowContext* context, Window* parentWindow);
    ~SettingGroupWindow() override;

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;
    void SetState(bool isCollapsed);
    bool IsCollapsed() const;

    void Initialize() override;
    Subscribeable<> OnSettingsToggled;
};
