#include "PrecompiledHeaders\stdafx.h"
#include "PageParser.h"

time_t PageParser::lastTkkRequestTime = -1;
long long PageParser::tkk1 = 0;
long long PageParser::tkk2 = 0;

long long PageParser::GetTkk1() 
{
	UpateTkkIfNeccessary();
	return PageParser::tkk1;
}

long long PageParser::GetTkk2()
{
	UpateTkkIfNeccessary();
	return PageParser::tkk2;
}

void PageParser::UpateTkkIfNeccessary()
{
	time_t  timev;
	time(&timev);

	if (PageParser::lastTkkRequestTime == -1 || timev - PageParser::lastTkkRequestTime > 60 * 60)
	{
		Logger::Log(L"Start requesting TKK.");

		wstring translatePageURL = L"https://translate.google.com";
		wstring translatePageMarkup = RequestHelper::GetStringResponse(translatePageURL);

		GumboOutput* output = gumbo_parse(Utilities::GetUtf8String(translatePageMarkup).c_str());
		string scriptContent = SearchScriptTag(output->root);

		if (scriptContent == "")
		{
			Logger::Log(L"Error. Unable to find script with TKK eval.");
			return;
		}

		duk_context *ctx = duk_create_heap_default();
		duk_push_c_function(ctx, ExtractTKK, DUK_VARARGS);
		duk_put_global_string(ctx, "getTKK");

		scriptContent += "; getTKK(TKK);";

		duk_eval_string_noresult(ctx, scriptContent.c_str());

		duk_destroy_heap(ctx);

		PageParser::lastTkkRequestTime = timev;

		Logger::Log(L"End requesting TKK.");
	}
}

string PageParser::SearchScriptTag(GumboNode* node) {

	if (node->type != GUMBO_NODE_ELEMENT)
	{
		return "";
	}

	if (node->v.element.tag == GUMBO_TAG_SCRIPT)
	{
		string scriptContent = static_cast<GumboNode*>(node->v.element.children.data[0])->v.text.text;

		if (scriptContent.find("TKK=") != std::string::npos)
		{
			return scriptContent;
		}
	}

	GumboVector* children = &node->v.element.children;
	for (unsigned int i = 0; i < children->length; ++i)
	{
		string result = SearchScriptTag(static_cast<GumboNode*>(children->data[i]));

		if (result != "")
		{
			return result;
		}
	}

	return "";
}

duk_ret_t PageParser::ExtractTKK(duk_context *ctx)
{
	const char* res = duk_to_string(ctx, 0);

	if (res == NULL)
	{
		return -1;
	}

	vector<string> parts = Split(res, '.');

	PageParser::tkk1 = atoll(parts[0].c_str());
	PageParser::tkk2 = atoll(parts[1].c_str());

	Logger::Log(L"TKK has been extracted. New values: Tkk1 = " + to_wstring(PageParser::tkk1) + L", Tkk2 = " + to_wstring(PageParser::tkk2) + L".");

	return 0;
}

vector<string> PageParser::Split(const string &s, char delim) {
	vector<string> elems;

	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}