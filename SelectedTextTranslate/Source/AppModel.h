#pragma once
#include "Entities\TranslateResult.h"
#include "TranslateEngine\Translator.h"
#include "TranslateEngine\TextPlayer.h"
#include "Loggers\DictionaryLogger.h"
#include "Helpers\TextExtractor.h"

class MainWindow;

class AppModel
{
private:
	TranslateResult translateResult;
	
	TextExtractor* textExtractor;	
	MainWindow* mainWindow;
	Translator* translator;
	TextPlayer* textPlayer;
	DictionaryLogger* dictionaryLogger;

public:
	AppModel(Translator* translator, TextPlayer* textPlayer, TextExtractor* textExtractor, DictionaryLogger* dictionaryLogger);
	~AppModel();

	void SetMainWindow(MainWindow* mainWindow);

	vector<LogRecord> GetDictionaryRecords();
	TranslateResult GetCurrentTranslateResult();

	void TranslateSelectedText();
	void ToggleTranslateResultDictionary(int translateResultDictionaryIndex);

	void PlaySelectedText();
	void PlayCurrentText();

	void ShowDictionary();
	void TranslateWordFromDictionary(int wordInDictionaryIndex);

	void Exit();
};