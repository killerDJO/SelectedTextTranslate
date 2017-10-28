#pragma once
#include "Presentation\Framework\Components\Component.h"
#include "Presentation\Components\Main\MainView.h"
#include "Presentation\Providers\TrayIcon.h"
#include "BusinessLogic\Translation\TextExtractor.h"
#include "Presentation\Components\Main\MainViewModel.h"

class MainComponent : public Component<MainView>, ModelHolder<MainViewModel*>
{
private:
    HotkeysRegistry* hotkeysRegistry;
    MessageBus* messageBus;
    TextExtractor* textExtractor;
    TextPlayer* textPlayer;
    TranslationService* translationService;
    ViewModelsStore* viewModelsStore;

    void ProcessHotkey(int hotkey) const;
    void ProcessVisibilityChange(bool isVisible);

    void PlaySelectedText() const;
    void TranslateSelectedText() const;
    void ShowApplicatonView(ApplicationViews applicationView);

public:
    MainComponent(ServiceRegistry* serviceRegistry);

    void SetLayout(LayoutDescriptor layout) override;
    MainViewModel* GetModel() override;
};