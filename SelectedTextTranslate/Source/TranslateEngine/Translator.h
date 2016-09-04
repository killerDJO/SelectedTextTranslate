#pragma once
#include "Entities\TranslateResult.h"
#include "Helpers\RequestProvider.h"
#include "Helpers\StringUtilities.h"
#include "TranslateEngine\TranslatePageParser.h"
#include "Loggers\Logger.h"

class Translator
{
private:
	RequestProvider* requestProvider;
	TranslatePageParser* translatePageParser;
	Logger* logger;

	TranslateResult ParseJSONResponse(wstring json);
	void ReplaceAll(wstring &str, const wstring &search, const wstring &replace);

public:
	Translator(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser);
	~Translator();

	TranslateResult TranslateSentence(wstring sentence);	
	wstring GetHash(wstring sentence);
};