#pragma once
#include "Model\Enums\ApplicationViews.h"
#include "Entities\TranslateResult.h"
#include "TranslateEngine\Translator.h"
#include "TranslateEngine\TextPlayer.h"
#include "Dictionary\Dictionary.h"
#include "Helpers\TextExtractor.h"

class MainWindow;

class AppModel
{
private:
    TranslateResult translateResult;
    ApplicactionViews applicationView;

    TextExtractor* textExtractor;
    MainWindow* mainWindow;
    Translator* translator;
    TextPlayer* textPlayer;
    Dictionary* dictionary;

public:
    AppModel(Translator* translator, TextPlayer* textPlayer, TextExtractor* textExtractor, Dictionary* dictionary);
    ~AppModel();

    void SetMainWindow(MainWindow* mainWindow);

    ApplicactionViews GetCurrentApplicationView() const;

    vector<LogRecord> GetDictionaryRecords() const;
    TranslateResult GetCurrentTranslateResult() const;

    void TranslateSelectedText();
    void ToggleTranslateResultDictionary(int translateResultDictionaryIndex);

    void PlaySelectedText();
    void PlayCurrentText() const;

    void ShowDictionary();
    void TranslateWordFromDictionary(int wordInDictionaryIndex);

    void Exit() const;
};