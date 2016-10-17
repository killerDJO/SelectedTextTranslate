#include "Controllers\AppController.h"

AppController::AppController(
    MainWindow* mainWindow,
    TrayIconProvider* trayIconProvider,
    TranslationService* translator,
    TextPlayer* textPlayer,
    TextExtractor* textExtractor,
    DictionaryService* dictionary)
{
    this->mainWindow = mainWindow;
    this->textPlayer = textPlayer;
    this->translationService = translator;
    this->textExtractor = textExtractor;
    this->dictionary = dictionary;
    this->trayIconProvider = trayIconProvider;
}

void AppController::Initialize()
{
    mainWindow->OnPlayText.Subscribe(bind(&AppController::PlayCurrentText, this));
    mainWindow->OnExpandTranslationResult.Subscribe(bind(&AppController::ToggleTranslateResultDictionary, this, placeholders::_1));
    mainWindow->OnShowTranslation.Subscribe(bind(&AppController::TranslateWordFromDictionary, this, placeholders::_1));

    trayIconProvider->OnExit.Subscribe(bind(&AppController::Exit, this));
    trayIconProvider->OnPlaySelectedText.Subscribe(bind(&AppController::PlaySelectedText, this));
    trayIconProvider->OnShowDictionary.Subscribe(bind(&AppController::ShowDictionary, this));
    trayIconProvider->OnTranslateSelectedText.Subscribe(bind(&AppController::TranslateSelectedText, this));
}

void AppController::TranslateSelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();

    translateResult = translationService->TranslateSentence(selectedText, true);

    mainWindow->SetCurrentView(ApplicationViews::TranslateResult);
    mainWindow->SetTranslateResultModel(translateResult);

    mainWindow->Render();
    mainWindow->Maximize();
}

void AppController::ToggleTranslateResultDictionary(int translateResultDictionaryIndex)
{
    translateResult.TranslateCategories[translateResultDictionaryIndex].IsExtendedList ^= true;
    mainWindow->SetTranslateResultModel(translateResult);
    mainWindow->Render(true);
}

void AppController::PlaySelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();
    translateResult = translationService->TranslateSentence(selectedText, false);
    textPlayer->PlayText(translateResult.Sentence.Origin);
}

void AppController::PlayCurrentText() const
{
    textPlayer->PlayText(translateResult.Sentence.Origin);
}

void AppController::ShowDictionary() const
{
    mainWindow->SetCurrentView(ApplicationViews::Dictionary);
    mainWindow->SetDictionaryModel(dictionary->GetTopRecords(200));

    mainWindow->Render();
    mainWindow->Maximize();
}

void AppController::TranslateWordFromDictionary(int wordInDictionaryIndex)
{
    vector<LogRecord> logRecords = dictionary->GetTopRecords(200);
    LogRecord logRecordToTranslate = logRecords[wordInDictionaryIndex];

    translateResult = translationService->TranslateSentence(logRecordToTranslate.Word, false);
    
    mainWindow->SetCurrentView(ApplicationViews::TranslateResult);
    mainWindow->SetTranslateResultModel(translateResult);

    mainWindow->Render();
}

void AppController::Exit() const
{
    PostQuitMessage(0);
}

AppController::~AppController()
{
}