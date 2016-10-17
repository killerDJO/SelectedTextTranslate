#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\DictionaryService.h"
#include "Providers\RequestProvider.h"
#include "Infrastructure\Logging\Logger.h"

class TranslationService
{
private:
    RequestProvider* requestProvider;
    TranslatePageParser* translatePageParser;
    Logger* logger;
    DictionaryService* dictionaryService;

    TranslateResult ParseJSONResponse(wstring json) const;
    void ReplaceAll(wstring &str, const wstring &search, const wstring &replace) const;

public:
    TranslationService(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser, DictionaryService* dictionary);
    ~TranslationService();

    TranslateResult TranslateSentence(wstring sentence, bool updateDictionary) const;
    wstring GetHash(wstring sentence) const;
};