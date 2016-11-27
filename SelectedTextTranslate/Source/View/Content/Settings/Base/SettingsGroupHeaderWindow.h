#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "View\Content\Settings\Base\Enums\SettingsGroupState.h"

class SettingsGroupHeaderWindow : public ContentWindow
{
private:
    SettingsGroupState state;
    wstring title;

    HBRUSH backgroundBrush;

protected:
    Size RenderContent(Renderer* renderer) override;
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    SettingsGroupHeaderWindow(WindowContext* context, Window* parentWindow);
    ~SettingsGroupHeaderWindow();

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, int width);

    void SetTitle(wstring title);
    wstring GetTitle() const;

    void SetState(SettingsGroupState state);
    SettingsGroupState GetState() const;

    void Initialize() override;

    Subscribeable<> OnSettingsToggled;
};