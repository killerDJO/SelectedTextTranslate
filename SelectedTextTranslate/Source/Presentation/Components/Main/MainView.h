#pragma once
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Controls\Dialogs\Confirm\ConfirmDialogControl.h"
#include "Presentation\Components\Main\Dto\ViewDescriptor.h"
#include "Presentation\Components\Dictionary\DictionaryComponent.h"
#include "Presentation\Components\Main\Enums\ApplicationViews.h"
#include "Presentation\Components\Translation\TranslationComponent.h"
#include "Presentation\Components\Settings\SettingsComponent.h"

class MainView : public View
{
private:
    TranslationComponent* translationComponent;
    DictionaryComponent* dictionaryComponent;
    SettingsComponent* settingsComponent;
    ConfirmDialogControl* confirmDialogWindow;

    ApplicationViews applicationView;
    map<ApplicationViews, ViewDescriptor> applicationViewDescriptors;
    Size minSize;

    void CreateChildComponents();
    void SetViewWindowDescriptor(IComponent* component, ApplicationViews view);

    void ScaleViewDescriptor(ApplicationViews applicationView, double scaleFactorAdjustment);

    IComponent* GetComponentToShow() const;

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);
    bool IsResizeLocked();

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainView(CommonContext* context);

    void Initialize() override;
    void Render(bool preserveScrolls = false) override;
    void Resize() override;

    void SetApplicationView(ApplicationViews applicationView);
    void Translate(wstring input);

    void SetLayout(LayoutDescriptor layoutDescriptor);

    void Minimize();
    void Maximize();

    void Scale(double scaleFactorAjustment);

    Subscribeable<int> OnHotkey;
    Subscribeable<bool> OnVisibilityChanged;
};
