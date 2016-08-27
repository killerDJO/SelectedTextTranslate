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
	static TranslateResult TranslateSentence(wstring sentence);
	static TranslateResult TranslateSelectedText();
	static wstring GetHash(wstring sentence);
private:
	static long long tkk1;
	static long long tkk2;
	static time_t lastTkkRequestTime;

	static TranslateResult ParseJSONResponse(wstring json);
	static void ReplaceAll(wstring &str, const wstring &search, const wstring &replace);

	static void UpateTkkIfNeccessary();
	static duk_ret_t  ExtractTKK(duk_context *ctx);
	static vector<string> Translator::Split(const string &s, char delim);
	static string Translator::SearchScriptTag(GumboNode* node);
};