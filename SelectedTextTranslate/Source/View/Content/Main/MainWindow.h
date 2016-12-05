#pragma once
#include "View\Framework\Windows\Window.h"
#include "View\Content\Dictionary\DictionaryWindow.h"
#include "View\Content\Translation\TranslationWindow.h"
#include "View\Content\Settings\SettingsWindow.h"
#include "Controllers\Enums\ApplicationViews.h"
#include "View\Content\Main\Dto\ViewDescriptor.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"

class MainWindow : public Window
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

    void CreateChildWindows();
    void SetViewWindowDescriptor(Window* viewWindow, ApplicationViews view);

    void Scale(double scaleFactorAjustment);
    void ScaleViewDescriptor(ApplicationViews applicationView, double scaleFactorAdjustment);
    void Resize() override;

    void SetCurrentView(ApplicationViews applicationView);
    Window* GetWindowToShow() const;

    void ShowConfirmDialog(wstring title, function<void()> onConfirm);
    bool IsResizeLocked();

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainWindow(WindowContext* context, HotkeyProvider* hotkeyProvider);
    ~MainWindow();

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
    Subscribeable<int> OnExpandTranslationResult;
    Subscribeable<int> OnShowTranslation;
    Subscribeable<Settings> OnSaveSettings;
};
