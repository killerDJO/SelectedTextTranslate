#pragma once
#include "Entities\TranslateResult.h"
#include "Helpers\RequestProvider.h"
#include "TranslateEngine\TranslatePageParser.h"
#include "Loggers\Logger.h"
#include "Dictionary\Dictionary.h"

class Translator
{
private:
    RequestProvider* requestProvider;
    TranslatePageParser* translatePageParser;
    Logger* logger;
    Dictionary* dictionary;

    TranslateResult ParseJSONResponse(wstring json) const;
    void ReplaceAll(wstring &str, const wstring &search, const wstring &replace) const;

public:
    Translator(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser, Dictionary* dictionary);
    ~Translator();

    TranslateResult TranslateSentence(wstring sentence, bool updateDictionary) const;
    wstring GetHash(wstring sentence) const;
};