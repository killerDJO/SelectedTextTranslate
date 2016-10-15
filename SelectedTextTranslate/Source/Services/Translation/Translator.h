#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\Dictionary.h"
#include "Providers\RequestProvider.h"
#include "Logging\Logger.h"

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