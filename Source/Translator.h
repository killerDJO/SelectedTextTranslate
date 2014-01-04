#pragma once
#include "stdafx.h"

struct TranslateResultWord
{
	string Word;
	vector<string> ReverseTranslation;
	double Score;
};

struct TranslateResultCategory
{
	string CategoryName;
	vector<TranslateResultWord> Words;
};

struct TranslateResult
{
	string Word;
	string TranslatedWord;

	vector<TranslateResultCategory> Categories;
};

static class Translator
{
public:
	static TranslateResult GetTranslationResults();	

	static string GetSelectedText();
	static string GetTraslatorJSONResponse(string text);
	static TranslateResult ParseJSONResponse(string json);
	static int Writer(void *data, size_t size, size_t nmemb, std::string *buffer_in);
};



