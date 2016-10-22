#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Providers\RequestProvider.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\DictionaryService.h"

class TranslationService
{
private:
    RequestProvider* requestProvider;
    TranslatePageParser* translatePageParser;
    Logger* logger;
    DictionaryService* dictionaryService;

    TranslateResult ParseJSONResponse(wstring json) const;

public:
    TranslationService(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser, DictionaryService* dictionary);
    ~TranslationService();

    TranslateResult TranslateSentence(wstring sentence, bool updateDictionary) const;
    wstring GetHash(wstring sentence) const;
};