#include "AppModel.h"
#include "Windows\MainWindow.h"

AppModel::AppModel(Translator* translator, TextPlayer* textPlayer, TextExtractor* textExtractor, Dictionary* dictionary)
{
    this->mainWindow = mainWindow;
    this->textPlayer = textPlayer;
    this->translator = translator;
    this->textExtractor = textExtractor;
    this->dictionary = dictionary;

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
    return dictionary->GetTopRecords(200);
}

TranslateResult AppModel::GetCurrentTranslateResult() const
{
    return translateResult;
}

void AppModel::TranslateSelectedText()
{
    wstring selectedText = textExtractor->GetSelectedText();

    translateResult = translator->TranslateSentence(selectedText, true);
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
    translateResult = translator->TranslateSentence(selectedText, false);
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
    vector<LogRecord> logRecords = dictionary->GetTopRecords(200);
    LogRecord logRecordToTranslate = logRecords[wordInDictionaryIndex];

    translateResult = translator->TranslateSentence(logRecordToTranslate.Word, false);
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