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
    ConfirmDialogControl* confirmDialog;

    ModelHolder<MainViewModel*>* modelHolder;
    map<ApplicationViews, IComponent*> viewToComponentMap;

    void CreateChildComponents();
    void InitializeComponent(IComponent* component, ApplicationViews view);
    void CreateConfirmDialog();

    IComponent* GetComponentToShow();
    MainViewModel* GetModel() const;

    bool IsResizeLocked() const;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    DWORD GetExtendedWindowStyles() const override;
    DWORD GetWindowStyle() const override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainView(ViewContext* context, ModelHolder<MainViewModel*>* modelHolder);

    void Initialize() override;
    void SetLayout(LayoutDescriptor layout);
    void Render(bool preserveScrolls = false) override;
    void Resize() override;
    void Scale(double scaleFactorAjustment);

    void Show() override;
    void ShowConfirmDialog(wstring title, function<void()> onConfirm);

    Subscribable<int> OnHotkey;
    Subscribable<bool> OnVisibilityChanged;
};