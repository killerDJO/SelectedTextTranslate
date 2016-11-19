#pragma once
#include "View\Content\Base\ContentWindow.h"

class SettingGroupWindow : public ContentWindow
{
private:
    bool isCollapsed;
    wstring title;
    int padding;

protected:
    Size RenderContent(Renderer* renderer) override;
    virtual void RenderSettingsContent(Renderer* renderer, Point contentPosition) = 0;

public:
    SettingGroupWindow(
        WindowContext* context,
        Point position,
        int width,
        wstring name,
        Window* parentWindow,
        bool isCollapsed,
        wstring title,
        int lineHeight,
        int padding = 5);

    ~SettingGroupWindow() override;

    void Initialize() override;
    Subscribeable<> OnSettingsToggled;
};
