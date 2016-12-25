#include "Services\Translation\TranslatePageParser.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Utilities\StringUtilities.h"

TranslatePageParser::TranslatePageParser(Logger* logger, RequestProvider* requestProvider)
{
    this->requestProvider = requestProvider;
    this->logger = logger;

    tkk1 = 0;
    tkk2 = 0;
    lastTkkRequestTime = -1;
}

long long TranslatePageParser::GetTkk1()
{
    UpateTkkIfNeccessary();
    return tkk1;
}

long long TranslatePageParser::GetTkk2()
{
    UpateTkkIfNeccessary();
    return tkk2;
}

void TranslatePageParser::UpateTkkIfNeccessary()
{
    time_t  timev;
    time(&timev);

    if (lastTkkRequestTime == -1 || timev - lastTkkRequestTime > 60 * 60)
    {
        logger->Log(LogLevels::Trace, L"Start requesting TKK.");

        wstring translatePageURL = L"https://translate.google.com";
        wstring translatePageMarkup = requestProvider->GetStringResponse(translatePageURL);

        GumboOutput* output = gumbo_parse(StringUtilities::GetUtf8String(translatePageMarkup).c_str());
        string scriptContent = SearchScriptTag(output->root);

        if (scriptContent == "")
        {
            throw SelectedTextTranslateException(L"Error. Unable to find script with TKK eval.");
        }

        duk_context *ctx = duk_create_heap_default();

        duk_eval_string_noresult(ctx, scriptContent.c_str());
        duk_eval_string(ctx, "TKK");
        const char* result = duk_get_string(ctx, -1);

        vector<wstring> parts = StringUtilities::Split(StringUtilities::GetUtf16StringFromChar(result), '.');

        tkk1 = _wtoll(parts[0].c_str());
        tkk2 = _wtoll(parts[1].c_str());

        logger->Log(LogLevels::Trace, L"TKK has been extracted. New values: Tkk1 = " + to_wstring(tkk1) + L", Tkk2 = " + to_wstring(tkk2) + L".");

        duk_destroy_heap(ctx);

        lastTkkRequestTime = timev;

        logger->Log(LogLevels::Trace, L"End requesting TKK.");
    }
}

string TranslatePageParser::SearchScriptTag(GumboNode* node) const
{

    if (node->type != GUMBO_NODE_ELEMENT)
    {
        return "";
    }

    if (node->v.element.tag == GUMBO_TAG_SCRIPT)
    {
        string scriptContent = static_cast<GumboNode*>(node->v.element.children.data[0])->v.text.text;

        if (scriptContent.find("TKK=") != string::npos)
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