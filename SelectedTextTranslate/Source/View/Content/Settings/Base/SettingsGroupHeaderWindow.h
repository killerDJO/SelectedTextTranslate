#pragma once
#include "View\Framework\Windows\ContentWindow.h"
#include "View\Content\Settings\Base\Enums\SettingsGroupVisibilityState.h"
#include "View\Content\Settings\Base\Enums\SettingsGroupContentState.h"

class SettingsGroupHeaderWindow : public ContentWindow
{
private:
    int borderWidth;

    SettingsGroupVisibilityState visibilityState;
    SettingsGroupContentState contentState;
    wstring title;

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

    void SetContentState(SettingsGroupContentState contentState);
    SettingsGroupContentState GetContentState() const;

    void SetVisibilityState(SettingsGroupVisibilityState visibilityState);
    SettingsGroupVisibilityState GetVisibilityState() const;

    void Initialize() override;

    Subscribeable<> OnSettingsToggled;
};
