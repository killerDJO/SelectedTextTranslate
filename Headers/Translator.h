#pragma once
#include "stdafx.h"
#include "TranslateResult.h"
#include "TextExtractor.h"
#include "RequestHelper.h"
#include "Utilities.h"

class Translator
{
public:
	static TranslateResult TranslateSentence(string sentence);
	static TranslateResult TranslateSelectedText();
private:
	static TranslateResult ParseJSONResponse(string json);
};