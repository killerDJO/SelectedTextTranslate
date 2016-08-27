#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResultDictionary.h"
#include "Entities\TranslateResultSentence.h"

class TranslateResult
{
public:
	TranslateResultSentence Sentence;
	vector<TranslateResultDictionary> TranslateCategories;
	TranslateResult();
	void Free();
};