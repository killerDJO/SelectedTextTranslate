#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResult.h"
#include "Helpers\TextExtractor.h"
#include "Helpers\RequestHelper.h"
#include "Helpers\Utilities.h"
#include "TranslateEngine\PageParser.h"
#include "Loggers\DictionaryLogger.h"
#include "Loggers\Logger.h"

class Translator
{
public:
	static TranslateResult TranslateSentence(wstring sentence);
	static TranslateResult TranslateSelectedText();
	static wstring GetHash(wstring sentence);
private:
	static TranslateResult ParseJSONResponse(wstring json);
	static void ReplaceAll(wstring &str, const wstring &search, const wstring &replace);
};