#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "Services\Translation\TranslationService.h"
#include "Services\Translation\TextPlayer.h"
#include "Services\Translation\TextExtractor.h"
#include "Services\Dictionary\DictionaryService.h"
#include "View\Content\MainWindow.h"
#include "View\Providers\TrayIconProvider.h"
#include "Services\Settings\SettingsProvider.h"

class AppController
{
private:
    TranslateResult translateResult;

    TextExtractor* textExtractor;
    MainWindow* mainWindow;
    TranslationService* translationService;
    TextPlayer* textPlayer;
    DictionaryService* dictionary;
    SettingsProvider* settingsProvider;

    TrayIconProvider* trayIconProvider;

public:
    AppController(
        MainWindow* mainWindow,
        TrayIconProvider* trayIconProvider,
        TranslationService* translator,
        TextPlayer* textPlayer,
        TextExtractor* textExtractor,
        DictionaryService* dictionaryService,
        SettingsProvider* settingsProvider);
    ~AppController();

    void Initialize();

    void TranslateSelectedText();
    void ForceTranslateCurrentText();
    void TranslateSuggestion();
    void ToggleTranslateResultCategory(int translateResultDictionaryIndex);

    void PlaySelectedText();
    void PlayCurrentText() const;

    void ShowDictionary() const;
    void ShowSettings() const;
    void TranslateWordFromDictionary(int wordInDictionaryIndex);

    void SaveSettings(Settings settings) const;

    void Exit() const;
};
