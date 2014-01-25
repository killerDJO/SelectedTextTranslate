#pragma once
#include "stdafx.h"
#include "TranslateResultDictionary.h"
#include "TranslateResultSentence.h"

struct TranslateResult
{
	TranslateResultSentence Sentence;
	vector<TranslateResultDictionary> TranslateCategories;
};