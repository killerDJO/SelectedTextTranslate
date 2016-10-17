#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "Services\Translation\TranslationService.h"
#include "Services\Translation\TextPlayer.h"
#include "Services\Dictionary\DictionaryService.h"
#include "Services\Translation\TextExtractor.h"
#include "View\Content\MainWindow.h"
#include "View\Providers\TrayIconProvider.h"

class AppController
{
private:
    TranslateResult translateResult;

    TextExtractor* textExtractor;
    MainWindow* mainWindow;
    TranslationService* translationService;
    TextPlayer* textPlayer;
    DictionaryService* dictionary;

    TrayIconProvider* trayIconProvider;
    HotkeyProvider* hotkeyProvider;

public:
    AppController(
        MainWindow* mainWindow,
        TrayIconProvider* trayIconProvider,
        HotkeyProvider* hotkeyProvider,
        TranslationService* translator,
        TextPlayer* textPlayer,
        TextExtractor* textExtractor,
        DictionaryService* dictionaryService);
    ~AppController();

    void Initialize();

    void TranslateSelectedText();
    void ToggleTranslateResultDictionary(int translateResultDictionaryIndex);

    void PlaySelectedText();
    void PlayCurrentText() const;

    void ShowDictionary() const;
    void TranslateWordFromDictionary(int wordInDictionaryIndex);

    void Exit() const;
};