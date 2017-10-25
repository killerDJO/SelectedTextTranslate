#pragma once
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogControl.h"
#include "Presentation\Components\Dictionary\DictionaryComponent.h"
#include "Presentation\Components\Main\Enums\ApplicationViews.h"
#include "Presentation\Components\Translation\TranslationComponent.h"
#include "Presentation\Components\Settings\SettingsComponent.h"
#include "Presentation\Components\Main\MainViewModel.h"

class MainView : public View
{
private:
    TranslationComponent* translationComponent;
    DictionaryComponent* dictionaryComponent;
    SettingsComponent* settingsComponent;
    ConfirmDialogControl* confirmDialog;

    ModelHolder<MainViewModel*>* modelHolder;
    map<ApplicationViews, IComponent*> viewToComponentMap;

    void CreateChildComponents();
    void InitializeComponent(IComponent* component, ApplicationViews view);

    IComponent* GetComponentToShow();
    MainViewModel* GetModel() const;

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);
    bool IsResizeLocked() const;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainView(CommonContext* context, ModelHolder<MainViewModel*>* modelHolder);

    void Initialize() override;
    void SetLayout(::LayoutDescriptor layout);
    void Render(bool preserveScrolls = false) override;
    void Resize() override;

    void Translate(wstring input) const;

    void Minimize();
    void Maximize();

    void Scale(double scaleFactorAjustment);

    Subscribeable<int> OnHotkey;
    Subscribeable<bool> OnVisibilityChanged;
};
