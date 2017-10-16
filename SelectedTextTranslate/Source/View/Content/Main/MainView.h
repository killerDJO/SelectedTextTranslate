#pragma once
#include "Controllers\Enums\ApplicationViews.h"
#include "View\Framework\View\Views\View.h"
#include "View\Providers\HotkeyProvider.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"
#include "View\Content\Main\Dto\ViewDescriptor.h"
#include "View\Content\Dictionary\DictionaryComponent.h"
#include "View\Content\Translation\TranslationView.h"
#include "View\Content\Settings\SettingsWindow.h"

class MainView : public View
{
private:
    TranslationView* translationWindow;
    DictionaryComponent* dictionaryComponent;
    SettingsWindow* settingsWindow;
    ConfirmDialogWindow* confirmDialogWindow;

    HotkeyProvider* hotkeyProvider;

    ApplicationViews applicationView;
    map<ApplicationViews, ViewDescriptor> applicationViewDescriptors;
    Size minSize;

    void CreateChildViews();
    void SetViewWindowDescriptor(View* viewWindow, ApplicationViews view);

    void Scale(double scaleFactorAjustment);
    void ScaleViewDescriptor(ApplicationViews applicationView, double scaleFactorAdjustment);
    void Resize() override;

    void SetApplicationView(ApplicationViews applicationView);
    View* GetViewToShow() const;

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);
    bool IsResizeLocked();

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainView(ViewContext* context, HotkeyProvider* hotkeyProvider);

    void Initialize() override;

    void SetDescriptor(WindowDescriptor descriptor) override;

    void Minimize();
    void Maximize();

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
    Subscribeable<int> OnShowTranslation;
    Subscribeable<Settings> OnSaveSettings;
};