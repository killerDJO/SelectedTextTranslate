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

    void SpecifyWindowClass(WNDCLASSEX* windowClass);

    void CreateChildComponents();
    void InitializeComponent(IComponent* component, ApplicationViews view);
    void CreateConfirmDialog();

    IComponent* GetComponentToShow();
    MainViewModel* GetModel() const;

    bool IsResizeLocked() const;
    void Resize();

    LRESULT ProcessSetCursor(WPARAM wParam, LPARAM lParam, function<LRESULT()> baseProcedure);
    LRESULT ProcessSysCommand(WPARAM wParam, LPARAM lParam, function<LRESULT()> baseProcedure);
    LRESULT ProcessActivate(WPARAM wParam, LPARAM lParam);
    LRESULT ProcessHotkey(WPARAM wParam, LPARAM lParam, function<LRESULT()> baseProcedure);
    LRESULT ProcessShowWindow(WPARAM wParam, LPARAM lParam);

protected:
    void RenderContent(Renderer* renderer) override;
    void SpecifyWindow(NativeWindowHolder* window) override;

public:
    MainView(ViewContext* context, ModelHolder<MainViewModel*>* modelHolder);

    void Initialize() override;
    void SetLayout(LayoutDescriptor layout) const;
    void Render(bool preserveScrolls = false) override;
    void Scale(double scaleFactorAjustment);

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);

    void Hide();

    Subscribable<int> OnHotkey;
    Subscribable<bool> OnVisibilityChanged;
};