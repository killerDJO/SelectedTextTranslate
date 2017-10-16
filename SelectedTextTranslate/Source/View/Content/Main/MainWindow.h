#pragma once
#include "Controllers\Enums\ApplicationViews.h"
#include "View\Framework\View\Views\View.h"
#include "View\Providers\HotkeyProvider.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"
#include "View\Content\Main\Dto\ViewDescriptor.h"
#include "View\Content\Dictionary\DictionaryWindow.h"
#include "View\Content\Translation\TranslationWindow.h"
#include "View\Content\Settings\SettingsWindow.h"

class MainWindow : public View
{
private:
    TranslationWindow* translationWindow;
    DictionaryWindow* dictionaryWindow;
    SettingsWindow* settingsWindow;
    ConfirmDialogWindow* confirmDialogWindow;

    HotkeyProvider* hotkeyProvider;

    ApplicationViews currentView;
    map<ApplicationViews, ViewDescriptor> viewDescriptors;
    Size minSize;

    TranslateResult translateResult;
    vector<DictionaryRecord> dictionaryRecords;
    Settings settings;

    void CreateChildViews();
    void SetViewWindowDescriptor(View* viewWindow, ApplicationViews view);

    void Scale(double scaleFactorAjustment);
    void ScaleViewDescriptor(ApplicationViews applicationView, double scaleFactorAdjustment);
    void Resize() override;

    void SetCurrentView(ApplicationViews applicationView);
    View* GetWindowToShow() const;

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);
    bool IsResizeLocked();

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainWindow(ViewContext* context, HotkeyProvider* hotkeyProvider);

    void Initialize() override;

    void SetDescriptor(WindowDescriptor descriptor) override;

    void Minimize();
    void Maximize();

    void SetTranslateResultView(TranslateResult translateResult);
    void SetDictionaryView(vector<DictionaryRecord> dictionaryRecords);
    void SetSettingsView(Settings settings);

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
    Subscribeable<int> OnShowTranslation;
    Subscribeable<Settings> OnSaveSettings;
};
