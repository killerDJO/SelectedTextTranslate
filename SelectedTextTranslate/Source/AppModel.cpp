#include "AppModel.h"
#include "Windows\MainWindow.h"

AppModel::AppModel(Translator* translator, TextPlayer* textPlayer, TextExtractor* textExtractor, DictionaryLogger* dictionaryLogger)
{
	this->mainWindow = mainWindow;
	this->textPlayer = textPlayer;
	this->translator = translator;
	this->textExtractor = textExtractor;
	this->dictionaryLogger = dictionaryLogger;
}

void AppModel::SetMainWindow(MainWindow* mainWindow)
{
	this->mainWindow = mainWindow;
}

vector<LogRecord> AppModel::GetDictionaryRecords()
{
	return dictionaryLogger->GetRecords();
}

TranslateResult AppModel::GetCurrentTranslateResult()
{
	return translateResult;
}

void AppModel::TranslateSelectedText()
{
	wstring selectedText = this->textExtractor->GetSelectedText();

	dictionaryLogger->AddRecord(selectedText);

	translateResult = translator->TranslateSentence(selectedText);
	mainWindow->ShowTranslateResultView();
	mainWindow->Maximize();
}

void AppModel::ToggleTranslateResultDictionary(int translateResultDictionaryIndex)
{
	translateResult.TranslateCategories[translateResultDictionaryIndex].IsExtendedList ^= true;
	mainWindow->ShowTranslateResultView(true);
}

void AppModel::PlaySelectedText()
{
	wstring selectedText = textExtractor->GetSelectedText();
	translateResult = translator->TranslateSentence(selectedText);
	textPlayer->PlayText(translateResult.Sentence.Origin);
}

void AppModel::PlayCurrentText()
{
	textPlayer->PlayText(translateResult.Sentence.Origin);
}

void AppModel::ShowDictionary()
{
	mainWindow->ShowDictionaryView();
}

void AppModel::TranslateWordFromDictionary(int wordInDictionaryIndex)
{
	vector<LogRecord> logRecords = dictionaryLogger->GetRecords();
	LogRecord logRecordToTranslate = logRecords[wordInDictionaryIndex];

	translateResult = translator->TranslateSentence(logRecordToTranslate.Word);
	mainWindow->ShowTranslateResultView();
}

void AppModel::Exit()
{
	PostQuitMessage(0);
}

AppModel::~AppModel()
{
}