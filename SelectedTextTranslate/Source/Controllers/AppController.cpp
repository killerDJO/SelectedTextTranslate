#include "Controllers\AppController.h"

AppController::AppController(
    MainWindow* mainWindow,
    TrayIconProvider* trayIconProvider,
    TranslationService* translator,
    TextPlayer* textPlayer,
    TextExtractor* textExtractor,
    DictionaryService* dictionary,
    SettingsProvider* settingsProvider,
    HotkeyProvider* hotkeyProvider)
{
    this->mainWindow = mainWindow;
    this->textPlayer = textPlayer;
    this->translationService = translator;
    this->textExtractor = textExtractor;
    this->dictionary = dictionary;
    this->trayIconProvider = trayIconProvider;
    this->settingsProvider = settingsProvider;
    this->hotkeyProvider = hotkeyProvider;
}

void AppController::Initialize()
{
    mainWindow->OnPlayText.Subscribe(bind(&AppController::PlayCurrentText, this));
    mainWindow->OnForceTranslation.Subscribe(bind(&AppController::ForceTranslateCurrentText, this));
    mainWindow->OnTranslateSuggestion.Subscribe(bind(&AppController::TranslateSuggestion, this));
    mainWindow->OnExpandTranslationResult.Subscribe(bind(&AppController::ToggleTranslateResultCategory, this, placeholders::_1));
    mainWindow->OnShowTranslation.Subscribe(bind(&AppController::TranslateWordFromDictionary, this, placeholders::_1));
    mainWindow->OnSaveSettings.Subscribe(bind(&AppController::SaveSettings, this, placeholders::_1));
    mainWindow->OnSettingsStateChanged.Subscribe(bind(&AppController::ShowSettings, this));

    trayIconProvider->OnExit.Subscribe(bind(&AppController::Exit, this));
    trayIconProvider->OnPlaySelectedText.Subscribe(bind(&AppController::PlaySelectedText, this));
    trayIconProvider->OnShowDictionary.Subscribe(bind(&AppController::ShowDictionary, this));
    trayIconProvider->OnShowSettings.Subscribe(bind(&AppController::ShowSettings, this));
    trayIconProvider->OnTranslateSelectedText.Subscribe(bind(&AppController::TranslateSelectedText, this));
}

void AppController::TranslateSelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();

    translateResult = translationService->TranslateSentence(selectedText, true, false);

    mainWindow->SetCurrentView(ApplicationViews::TranslateResult);
    mainWindow->SetTranslateResultModel(translateResult);

    mainWindow->Render();
    mainWindow->Maximize();
}

void AppController::ForceTranslateCurrentText()
{
    translateResult = translationService->TranslateSentence(translateResult.GetSentence().GetInput(), true, true);

    mainWindow->SetCurrentView(ApplicationViews::TranslateResult);
    mainWindow->SetTranslateResultModel(translateResult);
    mainWindow->Render();
}

void AppController::TranslateSuggestion()
{
    translateResult = translationService->TranslateSentence(translateResult.GetSentence().GetSuggestion(), true, false);

    mainWindow->SetCurrentView(ApplicationViews::TranslateResult);
    mainWindow->SetTranslateResultModel(translateResult);
    mainWindow->Render();
}

void AppController::ToggleTranslateResultCategory(int translateResultCategoryIndex)
{
    translateResult.ToggleCategory(translateResultCategoryIndex);
    mainWindow->SetTranslateResultModel(translateResult);
    mainWindow->Render(true);
}

void AppController::PlaySelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();
    translateResult = translationService->TranslateSentence(selectedText, false, false);
    textPlayer->PlayText(translateResult.GetSentence().GetOrigin());
}

void AppController::PlayCurrentText() const
{
    textPlayer->PlayText(translateResult.GetSentence().GetOrigin());
}

void AppController::ShowDictionary() const
{
    mainWindow->SetCurrentView(ApplicationViews::Dictionary);
    mainWindow->SetDictionaryModel(dictionary->GetTopRecords(200));

    mainWindow->Render();
    mainWindow->Maximize();
}

void AppController::ShowSettings() const
{
    mainWindow->SetCurrentView(ApplicationViews::Settings);
    mainWindow->SetSettingsModel(settingsProvider->GetSettings());

    mainWindow->Render();
    mainWindow->Maximize();
}

void AppController::TranslateWordFromDictionary(int wordInDictionaryIndex)
{
    vector<DictionaryRecord> dictionaryRecords = dictionary->GetTopRecords(200);
    DictionaryRecord recordToTranslate = dictionaryRecords[wordInDictionaryIndex];

    translateResult = translationService->TranslateSentence(recordToTranslate.GetWord(), false, false);

    mainWindow->SetCurrentView(ApplicationViews::TranslateResult);
    mainWindow->SetTranslateResultModel(translateResult);

    mainWindow->Render();
}

void AppController::SaveSettings(Settings settings) const
{
    settingsProvider->UpdateSettings(settings);
    hotkeyProvider->SetHotkeysSettings(settings.GetHotkeySettings());
    mainWindow->SetSettingsModel(settingsProvider->GetSettings());
    mainWindow->Render();
}

void AppController::Exit() const
{
    PostQuitMessage(0);
}

AppController::~AppController()
{
}