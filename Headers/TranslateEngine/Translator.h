#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResult.h"
#include "Helpers\TextExtractor.h"
#include "Helpers\RequestHelper.h"
#include "Helpers\Utilities.h"
#include "External\duk_config.h"
#include "External\duktape.h"
#include "gumbo.h"

class Translator
{
public:
	static TranslateResult TranslateSentence(string sentence);
	static TranslateResult TranslateSelectedText();
	static string GetHash(string sentence);
private:
	static long long tkk1;
	static long long tkk2;
	static time_t lastTkkRequestTime;

	static TranslateResult ParseJSONResponse(string json);
	static void ReplaceAll(string &str, const string &search, const string &replace);

	static void UpateTkkIfNeccessary();
	static duk_ret_t  ExtractTKK(duk_context *ctx);
	static vector<string> Translator::Split(const string &s, char delim);
	static string Translator::SearchScriptTag(GumboNode* node);
};