#pragma once
#include "Presentation\Framework\Component.h"
#include "Presentation\Components\Main\MainView.h"
#include "Presentation\Providers\TrayIconProvider.h"
#include "BusinessLogic\Translation\TextExtractor.h"

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
    MainComponent(CommonContext* context);
};
