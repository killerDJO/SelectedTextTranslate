#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Providers\RequestProvider.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\DictionaryService.h"

class TranslationService
{
private:
    const long long DictionaryRefreshInterval = 60 * 60 * 24 * 365; // 1 year

    RequestProvider* requestProvider;
    TranslatePageParser* translatePageParser;
    Logger* logger;
    DictionaryService* dictionaryService;

    TranslateResult ParseJSONResponse(wstring json) const;
    wstring GetTranslatorResponse(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const;
    wstring SendTranslationRequest(wstring sentence, bool forceTranslation) const;

public:
    TranslationService(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser, DictionaryService* dictionary);
    ~TranslationService();

    TranslateResult TranslateSentence(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const;
    wstring GetHash(wstring sentence) const;
};