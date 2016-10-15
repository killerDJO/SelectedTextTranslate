#include "Controller\AppController.h"
#include "View\Content\MainWindow.h"

AppController::AppController(Translator* translator, TextPlayer* textPlayer, TextExtractor* textExtractor, Dictionary* dictionary)
{
    this->mainWindow = mainWindow;
    this->textPlayer = textPlayer;
    this->translator = translator;
    this->textExtractor = textExtractor;
    this->dictionary = dictionary;

    this->applicationView = ApplicationViews::Hidden;
}

void AppController::SetMainWindow(MainWindow* mainWindow)
{
    this->mainWindow = mainWindow;
}

ApplicationViews AppController::GetCurrentApplicationView() const
{
    return applicationView;
}

vector<LogRecord> AppController::GetDictionaryRecords() const
{
    return dictionary->GetTopRecords(200);
}

TranslateResult AppController::GetCurrentTranslateResult() const
{
    return translateResult;
}

void AppController::TranslateSelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();

    translateResult = translator->TranslateSentence(selectedText, true);
    applicationView = ApplicationViews::TranslateResult;

    mainWindow->Render();
    mainWindow->Maximize();
}

void AppController::ToggleTranslateResultDictionary(int translateResultDictionaryIndex)
{
    translateResult.TranslateCategories[translateResultDictionaryIndex].IsExtendedList ^= true;
    applicationView = ApplicationViews::TranslateResult;
    mainWindow->Render(true);
}

void AppController::PlaySelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();
    translateResult = translator->TranslateSentence(selectedText, false);
    textPlayer->PlayText(translateResult.Sentence.Origin);
}

void AppController::PlayCurrentText() const
{
    textPlayer->PlayText(translateResult.Sentence.Origin);
}

void AppController::ShowDictionary()
{
    applicationView = ApplicationViews::Dictionary;
    mainWindow->Render();
    mainWindow->Maximize();
}

void AppController::TranslateWordFromDictionary(int wordInDictionaryIndex)
{
    vector<LogRecord> logRecords = dictionary->GetTopRecords(200);
    LogRecord logRecordToTranslate = logRecords[wordInDictionaryIndex];

    translateResult = translator->TranslateSentence(logRecordToTranslate.Word, false);
    applicationView = ApplicationViews::TranslateResult;
    mainWindow->Render();
}

void AppController::ShowError()
{
    applicationView = ApplicationViews::Error;
    mainWindow->Render();
    mainWindow->Maximize();
}

void AppController::Exit() const
{
    PostQuitMessage(0);
}

AppController::~AppController()
{
}