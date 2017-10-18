#pragma once
#include "View\Framework\View\Views\View.h"
#include "View\Providers\HotkeyProvider.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"
#include "View\Content\Main\Dto\ViewDescriptor.h"
#include "View\Content\Dictionary\DictionaryComponent.h"
#include "View\Content\Main\Enums\ApplicationViews.h"
#include "View\Content\Translation\TranslationComponent.h"
#include "View\Content\Settings\SettingsComponent.h"

class MainView : public View
{
private:
    TranslationComponent* translationWindow;
    DictionaryComponent* dictionaryComponent;
    SettingsComponent* settingsWindow;
    ConfirmDialogWindow* confirmDialogWindow;

    ApplicationViews applicationView;
    map<ApplicationViews, ViewDescriptor> applicationViewDescriptors;
    Size minSize;

    void CreateChildViews();
    void SetViewWindowDescriptor(View* viewWindow, ApplicationViews view);

    void ScaleViewDescriptor(ApplicationViews applicationView, double scaleFactorAdjustment);
    void Resize() override;

    View* GetViewToShow() const;

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);
    bool IsResizeLocked();

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainView(ViewContext* context);

    void Initialize() override;

    void SetApplicationView(ApplicationViews applicationView);
    void Translate(wstring input);

    void SetDescriptor(WindowDescriptor descriptor) override;

    void Minimize();
    void Maximize();

    void Scale(double scaleFactorAjustment);

    Subscribeable<int> OnHotkey;
    Subscribeable<bool> OnVisibilityChanged;
};
