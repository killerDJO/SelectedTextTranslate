#pragma once
#include "stdafx.h"
#include "TranslateResult.h"

class Translator
{
public:
	static TranslateResult TranslateSentence(string sentence);
	static TranslateResult TranslateSelectedText();
private:
	static TranslateResult ParseJSONResponse(string json);
};