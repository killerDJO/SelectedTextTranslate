#pragma once
#include "View\Framework\Windows\Window.h"
#include "View\Content\Dictionary\DictionaryWindow.h"
#include "View\Content\Translation\TranslationWindow.h"
#include "View\Content\Settings\SettingsWindow.h"
#include "View\Providers\HotkeyProvider.h"
#include "Controllers\Enums\ApplicationViews.h"

class MainWindow : public Window
{
private:
    HotkeyProvider* hotkeyProvider;

    TranslationWindow* translationWindow;
    DictionaryWindow* dictionaryWindow;
    SettingsWindow* settingsWindow;

    ApplicationViews currentView;

    TranslateResult translateResult;
    vector<DictionaryRecord> dictionaryRecords;

    void CreateViews();

    void Scale(double scaleFactorAjustment);
    void Resize() override;
    Window* GetWindowToShow() const;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainWindow(WindowContext* context, HotkeyProvider* hotkeyProvider);
    ~MainWindow();

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
    Subscribeable<int> OnExpandTranslationResult;
    Subscribeable<int> OnShowTranslation;

    void Initialize() override;

    void Minimize();
    void Maximize();

    void SetCurrentView(ApplicationViews applicationView);
    void SetTranslateResultModel(TranslateResult translateResult);
    void SetDictionaryModel(vector<DictionaryRecord> dictionaryRecords);
};