#pragma once
#include "Presentation\Framework\Components\Component.h"
#include "Presentation\Components\Main\MainView.h"
#include "Presentation\Providers\TrayIcon.h"
#include "Presentation\Components\Main\MainViewModel.h"

class MainComponent : public Component<MainView>, ModelHolder<MainViewModel*>
{
private:
    HotkeysRegistry* hotkeysRegistry;
    MessageBus* messageBus;
    ViewModelsStore* viewModelsStore;
    ScaleProvider* scaleProvider;
    ViewContext* commonContext;

    void ProcessHotkey(int hotkey) const;
    void ProcessVisibilityChange(bool isVisible);

    void ShowApplicatonView(ApplicationViews applicationView);

public:
    MainComponent(ServiceRegistry* serviceRegistry);
    ~MainComponent() override;

    void SetLayout(LayoutDescriptor layout) override;
    MainViewModel* GetModel() override;
};