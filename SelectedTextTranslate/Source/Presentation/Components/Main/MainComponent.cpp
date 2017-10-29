#include "Presentation\Components\Main\MainComponent.h"
#include "Presentation\Providers\TrayIcon.h"

MainComponent::MainComponent(ServiceRegistry* serviceRegistry)
    : Component(new MainView(serviceRegistry->Get<ViewContext>(), this))
{
    hotkeysRegistry = serviceRegistry->Get<HotkeysRegistry>();
    messageBus = serviceRegistry->Get<MessageBus>();
    viewModelsStore = serviceRegistry->Get<ViewModelsStore>();

    messageBus->OnShowDictionary.Subscribe(bind(&MainComponent::ShowApplicatonView, this, ApplicationViews::Dictionary));
    messageBus->OnShowSettings.Subscribe(bind(&MainComponent::ShowApplicatonView, this, ApplicationViews::Settings));
    messageBus->OnShowTranslation.Subscribe(bind(&MainComponent::ShowApplicatonView, this, ApplicationViews::TranslateResult));
    messageBus->OnConfirmRequested.Subscribe(bind(&MainView::ShowConfirmDialog, CurrentView, placeholders::_1, placeholders::_2));

    serviceRegistry->Get<ErrorHandler>()->OnErrorShow.Subscribe(bind(&MainView::Hide, CurrentView));

    CurrentView->OnHotkey.Subscribe(bind(&MainComponent::ProcessHotkey, this, placeholders::_1));
    CurrentView->OnVisibilityChanged.Subscribe(bind(&MainComponent::ProcessVisibilityChange, this, placeholders::_1));
}

void MainComponent::SetLayout(LayoutDescriptor layout)
{
    Component::SetLayout(layout);
    viewModelsStore->Store(new MainViewModel(ApplicationViews::None, layout));
}

MainViewModel* MainComponent::GetModel()
{
    return viewModelsStore->Get<MainViewModel>();
}

void MainComponent::ShowApplicatonView(ApplicationViews applicationView)
{
    GetModel()->SetApplicationView(applicationView);
    CurrentView->Render();
}

void MainComponent::ProcessHotkey(int hotkey) const
{
    hotkeysRegistry->ProcessHotkey(hotkey);
}

void MainComponent::ProcessVisibilityChange(bool isVisible)
{
    const double ScaleFactor = 0.05;
    if (isVisible)
    {
        hotkeysRegistry->RegisterZoomInHotkey(
            CurrentView->GetHandle(),
            [=]() -> void { CurrentView->Scale(ScaleFactor); });

        hotkeysRegistry->RegisterZoomOutHotkey(
            CurrentView->GetHandle(),
            [=]() -> void { CurrentView->Scale(-ScaleFactor); });
    }
    else
    {
        hotkeysRegistry->UnregisterZoomInHotkey(CurrentView->GetHandle());
        hotkeysRegistry->UnregisterZoomOutHotkey(CurrentView->GetHandle());
    }
}

MainComponent::~MainComponent()
{
    delete CurrentView;
}