#pragma once
#include "View\Content\Base\ContentWindow.h"

class SettingsGroupWindow : public ContentWindow
{
protected:
    bool isCollapsed;
    wstring title;

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
    void SetState(bool isCollapsed);
    bool IsCollapsed() const;

    void Initialize() override;
    Subscribeable<> OnSettingsToggled;
};
