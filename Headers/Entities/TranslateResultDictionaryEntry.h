#pragma once
#include "PrecompiledHeaders\stdafx.h"

struct TranslateResultDictionaryEntry
{
	wchar_t* Word;
	vector<wchar_t*> ReverseTranslation;
	double Score;
};