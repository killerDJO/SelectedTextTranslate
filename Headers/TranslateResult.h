#pragma once
#include "stdafx.h"
#include "TranslateResultDictionary.h"
#include "TranslateResultSentence.h"

class TranslateResult
{
public:
	TranslateResultSentence Sentence;
	vector<TranslateResultDictionary> TranslateCategories;
	TranslateResult();
	void Free();
};