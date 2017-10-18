#pragma once
#include "View\Framework\View\Component.h"
#include "View\Content\Main\MainView.h"
#include "View\Providers\TrayIconProvider.h"
#include "Services\Translation\TextExtractor.h"

class MainComponent : public Component<MainView>
{
private:
    HotkeyProvider* hotkeyProvider;
    TrayIconProvider* trayIconProvider;
    TextExtractor* textExtractor;
    TextPlayer* textPlayer;

    void ProcessHotkey(int hotkey) const;
    void ProcessVisibilityChange(bool isVisible);

    void PlaySelectedText() const;
    void TranslateSelectedText() const;
    void PlayCurrentText() const;
    void ShowApplicatonView(ApplicationViews applicationView) const;

    void Suspend() const;
    void Enable() const;
    void Exit() const;

public:
    MainComponent(ViewContext* context);
};
