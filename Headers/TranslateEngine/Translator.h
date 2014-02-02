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
private:
	static TranslateResult ParseJSONResponse(string json);
};