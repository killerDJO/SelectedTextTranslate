#pragma once
#include "Controllers\Enums\ApplicationViews.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Services\Translation\Translator.h"
#include "Services\Translation\TextPlayer.h"
#include "Services\Dictionary\Dictionary.h"
#include "Services\Translation\TextExtractor.h"

class MainWindow;

class AppController
{
private:
    TranslateResult translateResult;
    ApplicationViews applicationView;

    TextExtractor* textExtractor;
    MainWindow* mainWindow;
    Translator* translator;
    TextPlayer* textPlayer;
    Dictionary* dictionary;

public:
    AppController(Translator* translator, TextPlayer* textPlayer, TextExtractor* textExtractor, Dictionary* dictionary);
    ~AppController();

    void SetMainWindow(MainWindow* mainWindow);

    ApplicationViews GetCurrentApplicationView() const;

    vector<LogRecord> GetDictionaryRecords() const;
    TranslateResult GetCurrentTranslateResult() const;

    void TranslateSelectedText();
    void ToggleTranslateResultDictionary(int translateResultDictionaryIndex);

    void PlaySelectedText();
    void PlayCurrentText() const;

    void ShowDictionary();
    void TranslateWordFromDictionary(int wordInDictionaryIndex);

    void ShowError();

    void Exit() const;
};