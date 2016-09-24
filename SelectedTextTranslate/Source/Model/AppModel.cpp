#include "AppModel.h"
#include "Windows\MainWindow.h"

AppModel::AppModel(Translator* translator, TextPlayer* textPlayer, TextExtractor* textExtractor, DictionaryLogger* dictionaryLogger)
{
    this->mainWindow = mainWindow;
    this->textPlayer = textPlayer;
    this->translator = translator;
    this->textExtractor = textExtractor;
    this->dictionaryLogger = dictionaryLogger;

    this->applicationView = ApplicactionViews::Hidden;
}

void AppModel::SetMainWindow(MainWindow* mainWindow)
{
    this->mainWindow = mainWindow;
}

ApplicactionViews AppModel::GetCurrentApplicationView() const
{
    return applicationView;
}

vector<LogRecord> AppModel::GetDictionaryRecords() const
{
    return dictionaryLogger->GetRecords();
}

TranslateResult AppModel::GetCurrentTranslateResult() const
{
    return translateResult;
}

void AppModel::TranslateSelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();

    dictionaryLogger->AddRecord(selectedText);

    translateResult = translator->TranslateSentence(selectedText);
    applicationView = ApplicactionViews::TranslateResult;

    mainWindow->Render();
    mainWindow->Maximize();
}

void AppModel::ToggleTranslateResultDictionary(int translateResultDictionaryIndex)
{
    translateResult.TranslateCategories[translateResultDictionaryIndex].IsExtendedList ^= true;
    applicationView = ApplicactionViews::TranslateResult;
    mainWindow->Render(true);
}

void AppModel::PlaySelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();
    translateResult = translator->TranslateSentence(selectedText);
    textPlayer->PlayText(translateResult.Sentence.Origin);
}

void AppModel::PlayCurrentText() const
{
    textPlayer->PlayText(translateResult.Sentence.Origin);
}

void AppModel::ShowDictionary()
{
    applicationView = ApplicactionViews::Dictionary;
    mainWindow->Render();
    mainWindow->Maximize();
}

void AppModel::TranslateWordFromDictionary(int wordInDictionaryIndex)
{
    vector<LogRecord> logRecords = dictionaryLogger->GetRecords();
    LogRecord logRecordToTranslate = logRecords[wordInDictionaryIndex];

    translateResult = translator->TranslateSentence(logRecordToTranslate.Word);
    applicationView = ApplicactionViews::TranslateResult;
    mainWindow->Render();
}

void AppModel::Exit() const
{
    PostQuitMessage(0);
}

AppModel::~AppModel()
{
}