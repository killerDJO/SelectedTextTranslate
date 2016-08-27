#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Helpers\RequestHelper.h"
#include "Helpers\Utilities.h"
#include "Loggers\Logger.h"
#include "duk_config.h"
#include "duktape.h"
#include "gumbo.h"

class PageParser
{
public:
	static long long GetTkk1();
	static long long GetTkk2();
private:
	static long long tkk1;
	static long long tkk2;
	static time_t lastTkkRequestTime;

	static void UpateTkkIfNeccessary();
	static duk_ret_t  ExtractTKK(duk_context *ctx);
	static vector<string> Split(const string &s, char delim);
	static string SearchScriptTag(GumboNode* node);
};