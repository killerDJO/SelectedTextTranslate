#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Providers\RequestProvider.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\DictionaryService.h"

using namespace web;

class TranslationService
{
private:
    const long long DictionaryRefreshInterval = 60 * 60 * 24 * 365; // 1 year

    RequestProvider* requestProvider;
    TranslatePageParser* translatePageParser;
    Logger* logger;
    DictionaryService* dictionaryService;

    TranslateResult ParseJSONResponse(wstring json, wstring input) const;
    TranslateResultSentence ParseTranslateResultSentence(json::value root, wstring input) const;
    vector<TranslateResultCategory> ParseTranslateCategories(json::value root) const;

    TranslateResult GetTranslatorResponse(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const;
    TranslateResult SendRequestAndParse(wstring sentence, bool forceTranslation, function<void(wstring, wstring, bool)> dictionaryAction) const;
    wstring SendTranslationRequest(wstring sentence, bool forceTranslation) const;

public:
    TranslationService(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser, DictionaryService* dictionary);
    ~TranslationService();

    TranslateResult TranslateSentence(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const;
    wstring GetHash(wstring sentence) const;
};