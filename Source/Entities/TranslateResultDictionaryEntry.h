#pragma once
#include "PrecompiledHeaders\stdafx.h"

struct TranslateResultDictionaryEntry
{
	const wchar_t* Word;
	vector<const wchar_t*> ReverseTranslation;
	double Score;
};