#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResult.h"
#include "Helpers\TextExtractor.h"
#include "Helpers\RequestHelper.h"
#include "Helpers\Utilities.h"

class Translator
{
public:
	static TranslateResult TranslateSentence(string sentence);
	static TranslateResult TranslateSelectedText();
	static string GetHash(string sentence, int tkk);
private:
	static TranslateResult ParseJSONResponse(string json);
	static void ReplaceAll(string &str, const string &search, const string &replace);
};